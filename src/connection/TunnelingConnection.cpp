#include "TunnelingConnection.h"
#include "ByteBufferReader.h"
#include "ConnectRequest.h"
#include "ConnectionRequestInformation.h"
#include "KnxIpHeader.h"
#include "TunnelingRequest.h"
#include "ConnectStateRequest.h"
#include "DisconnectRequest.h"
#include "ConnectResponse.h"
#include "ConnectStateResponse.h"
#include "DisconnectResponse.h"
#include "TunnelAckResponse.h"
#include <asio/as_tuple.hpp>
#include <asio/awaitable.hpp>
#include <asio/detached.hpp>
#include <asio/io_context.hpp>
#include <asio/ip/address_v4.hpp>
#include <asio/use_awaitable.hpp>
#include <chrono>
#include <cstdint>
#include <functional>
#include <ios>
#include <iostream>

namespace connection {

using namespace knx::requestresponse;

TunnelingConnection::TunnelingConnection(asio::io_context &ctx,
                                         std::string_view remoteIp,
                                         std::uint16_t remotePort,
                                         std::string_view localBindIp,
                                         std::uint16_t localDataPort,
                                         std::uint16_t localControlPort)
    : ctx{ctx}, checkConnectionTimer{ctx},
      remoteControlEndPoint{asio::ip::make_address_v4(remoteIp), remotePort},
      localBindIp{asio::ip::make_address_v4(localBindIp)},
      dataPort{localDataPort}, controlPort{localControlPort} {
  dataSocket =
      std::make_unique<udp::UdpSocket>(ctx, localBindIp, localDataPort);
  controlSocket =
      std::make_unique<udp::UdpSocket>(ctx, localBindIp, localControlPort);
  dataSocket->setHandler(
      std::bind_front(&TunnelingConnection::onReceiveData, this));
  controlSocket->setHandler(
      std::bind_front(&TunnelingConnection::onReceiveData, this));
}

TunnelingConnection::~TunnelingConnection() {
  if (dataSocket) {
    dataSocket->stop();
    controlSocket->stop();
  }
}

asio::awaitable<void>
TunnelingConnection::sendRequest(std::vector<std::uint8_t> &request,
                                 const std::uint16_t responseServiceId,
                                 CallBackFunction &&callBackFunction) {
  listeners[responseServiceId] = std::move(callBackFunction);
  co_await controlSocket->writeTo(remoteControlEndPoint, request);
}

void TunnelingConnection::addListener(ConnectionListener &listener) {
  connectionListeners.push_back(&listener);
}

asio::awaitable<void> TunnelingConnection::start() {
  controlSocket->start();
  dataSocket->start();

  listeners.emplace(
      TunnelRequest::SERVICE_ID,
      std::bind_front(&TunnelingConnection::onReceiveTunnelRequest, this));
  listeners.emplace(
      DisconnectRequest::SERVICE_ID,
      std::bind_front(&TunnelingConnection::onReceiveDisconnectRequest, this));

  // send connect request
  auto controlHpai = createControlHPAI();
  auto dataHpai = createDataHPAI();
  auto cri = ConnectionRequestInformation::newTunneling();
  ConnectRequest connectRequest{std::move(controlHpai), std::move(dataHpai),
                                std::move(cri)};
  auto bytes = connectRequest.toBytes();
  co_await sendRequest(
      bytes, knx::requestresponse::ConnectResponse::SERVICE_ID,
      [this](KnxIpHeader &, ByteBufferReader &data) {
        const ConnectResponse connectResponse = ConnectResponse::parse(data);
        channelId = connectResponse.getChannelId();
        auto ip = connectResponse.getDataEndPoint().getAddress().asString();
        auto port = connectResponse.getDataEndPoint().getPort();
        remoteDataEndPoint = {asio::ip::make_address_v4(ip), port};
        asio::co_spawn(ctx, checkConnection(), asio::detached);
        forEveryListener(
            [](ConnectionListener *listener) { listener->onConnect(); });
        return false;
      });
}

asio::awaitable<void> TunnelingConnection::checkConnection() {
  bool keepGoing = true;
  while (keepGoing) {
    checkConnectionTimer.expires_after(std::chrono::seconds(60));
    auto [errorcode] = co_await checkConnectionTimer.async_wait(asio::as_tuple);
    // if cancelled, stop
    if (errorcode) {
      keepGoing = false;
      break;
    }
    // send a connect state request
    ConnectStateRequest request{createControlHPAI(), channelId};
    auto bytes = request.toBytes();
    co_await sendRequest(
        bytes, knx::requestresponse::ConnectStateResponse::SERVICE_ID,
        [this, &keepGoing](KnxIpHeader &, ByteBufferReader &data) {
          ConnectStateResponse response = ConnectStateResponse::parse(data);
          if (response.getStatus()) {
            std::cout << "Got an error (" << (int)response.getStatus()
                      << ") closing.\n";
            co_spawn(ctx, this->close(), asio::detached);
            keepGoing = false;
          }
          return false;
        });
  }
}

auto TunnelingConnection::onReceiveTunnelRequest(KnxIpHeader &knxIpHeader,
                                                 ByteBufferReader &reader)
    -> bool {
  // send a acknowlegde
  const TunnelRequest request = TunnelRequest::parse(reader);
  ConnectionHeader header{request.getConnectionHeader()};
  TunnelAckResponse response{std::move(header)};
  auto tunnelResponseBytes = response.toBytes();
  controlSocket->writeToSync(remoteControlEndPoint, tunnelResponseBytes);

  Cemi cemi = request.getCemi();
  forEveryListener([&cemi](ConnectionListener *listener) {
    listener->onIncommingCemi(cemi);
  });

  return true;
}

auto TunnelingConnection::onReceiveDisconnectRequest(KnxIpHeader &knxIpHeader,
                                                     ByteBufferReader &reader)
    -> bool {
  const DisconnectRequest request = DisconnectRequest::parse(reader);
  if (request.getChannel() == channelId) {
    DisconnectResponse response{channelId};
    auto responseBytes = response.toBytes();
    controlSocket->writeToSync(remoteControlEndPoint, responseBytes);
    co_spawn(ctx, this->close(false), asio::detached);
  }
  return true;
}

void TunnelingConnection::onReceiveData(std::vector<std::uint8_t> &&data) {
  ByteBufferReader reader(data);
  KnxIpHeader knxIpHeader = KnxIpHeader::parse(reader);
  if (this->listeners.contains(knxIpHeader.getServiceType())) {
    const bool keep =
        this->listeners.at(knxIpHeader.getServiceType())(knxIpHeader, reader);
    if (!keep) {
      this->listeners.erase(knxIpHeader.getServiceType());
    }
  }
}

void TunnelingConnection::send(Cemi &&cemi) {
  static std::uint8_t sequence{0};

  // IP Tunneling: TunnelRequest in a ConnectionHeader
  ConnectionHeader connectionHeader{channelId, sequence++, 0x00};
  TunnelRequest tunnelRequest{std::move(connectionHeader), std::move(cemi)};
  auto bytes = tunnelRequest.toBytes();

  co_spawn(ctx, dataSocket->writeTo(remoteDataEndPoint, bytes), asio::detached);
  // the server should response with a TunnelAckResponse
  // indicated it has been recieved
  // if not, within a timeout, resend, again timeout-> disconnect
}

ConnectionRequestInformation
TunnelingConnection::createConnectRequestInformation() {
  return ConnectionRequestInformation::newTunneling();
}

HPAI TunnelingConnection::createDataHPAI() {
  return {IpAddress{this->localBindIp.to_bytes()}, this->dataPort, HPAI::UDP};
}

HPAI TunnelingConnection::createControlHPAI() {
  return {IpAddress{this->localBindIp.to_bytes()}, this->controlPort,
          HPAI::UDP};
};

void TunnelingConnection::forEveryListener(
    std::function<auto(ConnectionListener *)->void> doThis) {
  for (auto listenerRef : connectionListeners) {
    doThis(listenerRef);
  }
}

asio::awaitable<void> TunnelingConnection::close() { return close(true); }
asio::awaitable<void> TunnelingConnection::close(bool needsDisconnectRequest) {
  if (!closingDown) {
    closingDown = true;
    if (needsDisconnectRequest) {

      DisconnectRequest disconnectRequest{channelId, createControlHPAI()};
      auto requestBytes = disconnectRequest.toBytes();
      using namespace std::literals;

      asio::steady_timer timer(ctx);
      timer.expires_after(std::chrono::milliseconds(400));
      co_await sendRequest(
          requestBytes, DisconnectResponse::SERVICE_ID,
          [this, &timer](KnxIpHeader &knxIpHeader, ByteBufferReader &reader) {
            timer.cancel();
            return false;
          });
      auto [error] = co_await timer.async_wait(asio::as_tuple);
    }
    checkConnectionTimer.cancel();
    controlSocket->stop();
    dataSocket->stop();
    controlSocket.reset();
    dataSocket.reset();
    forEveryListener(
        [](ConnectionListener *listener) { listener->onDisconnect(); });
  }
}

} // namespace connection
