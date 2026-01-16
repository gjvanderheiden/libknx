#include "knx/connection/TunnelingConnection.h"
#include "knx/bytes/ByteBufferReader.h"
#include "knx/headers/ConnectionRequestInformation.h"
#include "knx/headers/KnxAddress.h"
#include "knx/headers/KnxIpHeader.h"
#include "knx/ip/UdpSocket.h"
#include "knx/requests//DisconnectRequest.h"
#include "knx/requests/ConnectRequest.h"
#include "knx/requests/ConnectStateRequest.h"
#include "knx/requests/DescriptionRequest.h"
#include "knx/requests/TunnelingRequest.h"
#include "knx/responses/ConnectResponse.h"
#include "knx/responses/ConnectStateResponse.h"
#include "knx/responses/DescriptionResponse.h"
#include "knx/responses/DisconnectResponse.h"
#include "knx/responses/TunnelAckResponse.h"
#include <asio/as_tuple.hpp>
#include <asio/awaitable.hpp>
#include <asio/co_spawn.hpp>
#include <asio/detached.hpp>
#include <asio/io_context.hpp>
#include <asio/ip/address_v4.hpp>
#include <asio/steady_timer.hpp>
#include <asio/use_awaitable.hpp>
#include <chrono>
#include <cstdint>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <ranges>

namespace knx::connection {

using namespace knx::requestresponse;
using namespace std::chrono_literals;

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
  dataSocket->setConnectionClosedHandler(
      std::bind_front(&TunnelingConnection::onDataSocketClosed, this));
  controlSocket->setConnectionClosedHandler(
      std::bind_front(&TunnelingConnection::onControlSocketClosed, this));
  listeners.emplace(
      TunnelRequest::SERVICE_ID,
      std::bind_front(&TunnelingConnection::onReceiveTunnelRequest, this));
  listeners.emplace(
      DisconnectRequest::SERVICE_ID,
      std::bind_front(&TunnelingConnection::onReceiveDisconnectRequest, this));
  listeners.emplace(
      TunnelAckResponse::SERVICE_ID,
      std::bind_front(&TunnelingConnection::onReceiveAckTunnelResponse, this));
}

TunnelingConnection::~TunnelingConnection() {
  if (dataSocket) {
    dataSocket->stop();
    controlSocket->stop();
  }
}

asio::awaitable<void>
TunnelingConnection::sendRequest(AbstractRequest& request,
                                 const std::uint16_t responseServiceId,
                                 CallBackFunction &&callBackFunction) {
  std::cout << "TunnelingConnection::sendRequest() serviceId = "
            << responseServiceId << "\n";
  std::vector<std::uint8_t> data;
  ByteBufferWriter writer{data};
  request.write(writer);
  listeners[responseServiceId] = std::move(callBackFunction);
  co_await controlSocket->writeTo(remoteControlEndPoint, data);
  std::cout << "TunnelingConnection::sendRequest() : done\n";
}

void TunnelingConnection::addListener(ConnectionListener &listener) {
  connectionListeners.push_back(&listener);
}

asio::awaitable<void> TunnelingConnection::start() {
  if (!controlSocket) {
    dataSocket = std::make_unique<udp::UdpSocket>(ctx, localBindIp.to_string(),
                                                  dataPort);
    controlSocket = std::make_unique<udp::UdpSocket>(
        ctx, localBindIp.to_string(), controlPort);
    dataSocket->setHandler(
        std::bind_front(&TunnelingConnection::onReceiveData, this));
    controlSocket->setHandler(
        std::bind_front(&TunnelingConnection::onReceiveData, this));
    dataSocket->setConnectionClosedHandler(
        std::bind_front(&TunnelingConnection::onDataSocketClosed, this));
    controlSocket->setConnectionClosedHandler(
        std::bind_front(&TunnelingConnection::onControlSocketClosed, this));
    listeners.clear();
    listeners.emplace(
        TunnelRequest::SERVICE_ID,
        std::bind_front(&TunnelingConnection::onReceiveTunnelRequest, this));
    listeners.emplace(
        DisconnectRequest::SERVICE_ID,
        std::bind_front(&TunnelingConnection::onReceiveDisconnectRequest,
                        this));
    listeners.emplace(
        TunnelAckResponse::SERVICE_ID,
        std::bind_front(&TunnelingConnection::onReceiveAckTunnelResponse,
                        this));
  }
  controlSocket->start();
  dataSocket->start();
  sequence = 0;

  // send connect request
  auto controlHpai = createControlHPAI();
  auto dataHpai = createDataHPAI();
  auto cri = ConnectionRequestInformation::newTunneling();
  ConnectRequest connectRequest{std::move(controlHpai), std::move(dataHpai),
                                std::move(cri)};
  co_await sendRequest(
      connectRequest, knx::requestresponse::ConnectResponse::SERVICE_ID,
      [this](KnxIpHeader &, ByteBufferReader &data) {
        std::cout << "got a connect response\n";
        const ConnectResponse connectResponse = ConnectResponse::parse(data);
        channelId = connectResponse.getChannelId();
        auto remoteIp =
            connectResponse.getDataEndPoint().getAddress().asString();
        auto port = connectResponse.getDataEndPoint().getPort();
        remoteDataEndPoint = {asio::ip::make_address_v4(remoteIp), port};
        asio::co_spawn(ctx, checkConnection(), asio::detached);
        knxAddress = connectResponse.getCRD().getAddress();
        forEveryListener(
            [](ConnectionListener *listener) { listener->onConnect(); });
        return false;
      });
  std::cout << "Tunneling started\n";
}

asio::awaitable<void> TunnelingConnection::checkConnection() {
  bool keepGoing = true;
  while (keepGoing) {
    checkConnectionTimer.expires_after(
        std::chrono::seconds(TIME_BETWEEN_CHECK_CONNECTION));
    auto [errorcode] = co_await checkConnectionTimer.async_wait(asio::as_tuple);
    // if cancelled, stop
    if (errorcode) {
      std::cout
          << "TunnelingConnection::checkConnection() : Got an timer error ("
          << errorcode << ") closing.\n";
      keepGoing = false;
      break;
    }
    // send a connect state request
    std::cout << "Sending connect state request\n";
    ConnectStateRequest request{createControlHPAI(), channelId};
    co_await sendRequest(
        request, knx::requestresponse::ConnectStateResponse::SERVICE_ID,
        [this, &keepGoing](KnxIpHeader &, ByteBufferReader &data) {
          ConnectStateResponse response = ConnectStateResponse::parse(data);
          if (response.getStatus()) {
            std::cout << "Got an error ("
                      << static_cast<int>(response.getStatus())
                      << ") closing.\n";
            co_spawn(ctx, this->close(), asio::detached);
            keepGoing = false;
          }
          return false;
        });
  }
}

auto TunnelingConnection::onReceiveTunnelRequest(KnxIpHeader & /*knxIpHeader*/,
                                                 ByteBufferReader &reader)
    -> bool {
  const TunnelRequest request = TunnelRequest::parse(reader);

  // send an acknowlegde
  {
    TunnelAckResponse response{ConnectionHeader{request.getConnectionHeader()}};
    auto tunnelResponseBytes = response.toBytes();
    controlSocket->writeToSync(remoteControlEndPoint, tunnelResponseBytes);
  }

  Cemi cemi = request.getCemi();
  forEveryListener([&cemi](ConnectionListener *listener) {
    listener->onIncommingCemi(cemi);
  });

  return true;
}

auto TunnelingConnection::onReceiveDisconnectRequest(
    KnxIpHeader & /*knxIpHeader*/, ByteBufferReader &reader) -> bool {
  std::cout << "Received disconnect request\n";
  const DisconnectRequest request = DisconnectRequest::parse(reader);
  if (request.getChannel() == channelId) {
    DisconnectResponse response{channelId};
    auto responseBytes = response.toBytes();
    controlSocket->writeToSync(remoteControlEndPoint, responseBytes);
    this->reset();

    forEveryListener(
        [](ConnectionListener *listener) { listener->onDisconnect(); });
  }
  return true;
}

auto TunnelingConnection::onReceiveAckTunnelResponse(
    KnxIpHeader & /*knxIpHeader*/, ByteBufferReader &reader) -> bool {
  const TunnelAckResponse response = TunnelAckResponse::parse(reader);
  if (response.getConnectionHeader().getChannel() == channelId) {
    auto sequenceAck = response.getConnectionHeader().getSequence();
    if (sendItems.contains(sequenceAck)) {
      sendItems[sequenceAck]->onReceiveAckTunnelResponse(
          response.getConnectionHeader());
    }
  }
  return true;
}

void TunnelingConnection::onReceiveData(std::vector<std::uint8_t> &&data) {
  std::cout << "TunnelingConnection::onReceiveData(std::vector<std::uint8_t> "
               "&&data)\n";
  ByteBufferReader reader(data);
  KnxIpHeader knxIpHeader = KnxIpHeader::parse(reader);

  std::cout << "TunnelingConnection::onReceiveData() : service type =  "
            << knxIpHeader.getServiceType() << "\n";
  if (this->listeners.contains(knxIpHeader.getServiceType())) {
    const bool keep =
        this->listeners.at(knxIpHeader.getServiceType())(knxIpHeader, reader);
    if (!keep) {
      this->listeners.erase(knxIpHeader.getServiceType());
    }
  }
}

asio::awaitable<void> TunnelingConnection::send(Cemi &&cemi) {
  std::uint8_t sequenceSend = sequence++;
  ConnectionHeader conectionHeader{channelId, sequenceSend};
  TunnelRequest tunnelRequest{std::move(conectionHeader), std::move(cemi)};
  std::vector<byte> data;
  ByteBufferWriter writer{data};
  tunnelRequest.write(writer);
  sendItems[sequenceSend] = std::make_unique<SendTunnelingState>(
      ctx, [this, &data]() -> asio::awaitable<void> {
        co_await this->dataSocket->writeTo(this->remoteDataEndPoint, data);
      });
  co_await sendItems[sequenceSend]->send();
  sendItems.erase(sequenceSend);
}

namespace {
IpAddress toIpAddress(const asio::ip::address_v4 &address) {
  auto toBytes = address.to_bytes();
  ByteBufferReader reader{toBytes};
  return IpAddress{reader.get4BytesCopy()};
}
} // namespace
  
HPAI TunnelingConnection::createDataHPAI() const {
  return {toIpAddress(this->localBindIp), this->dataPort, HPAI::UDP};
}

HPAI TunnelingConnection::createControlHPAI() const {
  return {toIpAddress(this->localBindIp), this->controlPort, HPAI::UDP};
}

void TunnelingConnection::forEveryListener(
    const std::function<auto(ConnectionListener *)->void> &doThis) const {
  for (const auto &listenerRef : connectionListeners) {
    doThis(listenerRef);
  }
}

asio::awaitable<void> TunnelingConnection::printDescription() {
  DescriptionRequest request{createControlHPAI()};

  asio::steady_timer timer(ctx);
  timer.expires_after(std::chrono::milliseconds(400));
  co_await sendRequest(
      request, DescriptionResponse::SERVICE_ID,
      [&timer](KnxIpHeader & /*knxIpHeader*/, ByteBufferReader &reader) {
        timer.cancel();
        const auto response = DescriptionResponse::parse(reader);
        std::cout << "supported service families ("
                  << response.getSupportedServiceFamiliesDib()
                         .getServiceFamilies()
                         .size()
                  << "):\n";
        for (auto fam :
             response.getSupportedServiceFamiliesDib().getServiceFamilies()) {
          std::cout << fam.toString() << '\n';
        }
        return false;
      });
  auto [error] = co_await timer.async_wait(asio::as_tuple);
}

void TunnelingConnection::onControlSocketClosed() {
  std::cerr << "control socket closed\n";
  asio::co_spawn(ctx, close(false), asio::detached);
}

void TunnelingConnection::onDataSocketClosed() {
  std::cerr << "data socket closed\n";
  asio::co_spawn(ctx, close(false), asio::detached);
}

void TunnelingConnection::reset() {
  checkConnectionTimer.cancel();
  for (const auto &sendTunnelingState : this->sendItems | std::views::values) {
    sendTunnelingState->cancel();
  }
  this->sendItems.clear();
  this->sequence = 0;
  this->knxAddress = {0, 0, 0};
}

const IndividualAddress &TunnelingConnection::getKnxAddress() {
  return knxAddress;
}

asio::awaitable<void> TunnelingConnection::close() { return close(true); }

asio::awaitable<void> TunnelingConnection::close(bool needsDisconnectRequest) {
  if (!closingDown) {
    closingDown = true;
    if (needsDisconnectRequest) {

      DisconnectRequest disconnectRequest{channelId, createControlHPAI()};

      asio::steady_timer timer(ctx);
      timer.expires_after(std::chrono::milliseconds(400));
      co_await sendRequest(disconnectRequest, DisconnectResponse::SERVICE_ID,
                           [&timer](KnxIpHeader &, ByteBufferReader &) {
                             timer.cancel();
                             return false;
                           });
      auto [error] = co_await timer.async_wait(asio::as_tuple);
    }
    this->reset();
    controlSocket->stop();
    dataSocket->stop();
    closingDown = false;
    forEveryListener(
        [](ConnectionListener *listener) { listener->onDisconnect(); });
  }
}

} // namespace knx::connection
