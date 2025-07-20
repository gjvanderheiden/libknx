#include "IpKnxConnection.h"
#include "ByteBufferReader.h"
#include "ConnectRequest.h"
#include "ConnectionRequestInformation.h"
#include "KnxAddress.h"
#include "KnxIpHeader.h"
#include "TunnelingRequest.h"
#include "cemi/ACPI.h"
#include "requests/ConnectStateRequest.h"
#include "requests/DisconnectRequest.h"
#include "responses/ConnectResponse.h"
#include "responses/ConnectStateResponse.h"
#include "responses/DisconnectResponse.h"
#include "responses/TunnelAckResponse.h"
#include <asio/as_tuple.hpp>
#include <asio/awaitable.hpp>
#include <asio/detached.hpp>
#include <asio/io_context.hpp>
#include <asio/ip/address_v4.hpp>
#include <asio/use_awaitable.hpp>
#include <chrono>
#include <cstdint>
#include <functional>
#include <iostream>

namespace connection {

using namespace knx::requestresponse;

IpKnxConnection::IpKnxConnection(asio::io_context &ctx,
                                 std::string_view remoteIp,
                                 std::uint16_t remotePort,
                                 std::string_view localBindIp,
                                 std::uint16_t localDataPort,
                                 std::uint16_t localControlPort)
    : ctx{ctx}, checkConnectionTimer{ctx},
      remoteControlEndPoint{asio::ip::make_address_v4(remoteIp), remotePort},
      localBindIp{asio::ip::make_address_v4(localBindIp)},
      dataPort{localDataPort}, controlPort{localControlPort},
      dataSocket{ctx, localBindIp, localDataPort},
      controlSocket{ctx, localBindIp, localControlPort} {
  dataSocket.setHandler(std::bind_front(&IpKnxConnection::onReceiveData, this));
  controlSocket.setHandler(
      std::bind_front(&IpKnxConnection::onReceiveData, this));
}

IpKnxConnection::~IpKnxConnection() {
  dataSocket.stop();
  controlSocket.stop();
}

asio::awaitable<void>
IpKnxConnection::sendRequest(std::vector<std::uint8_t> &request,
                             const std::uint16_t responseServiceId,
                             CallBackFunction &&callBackFunction) {
  listeners[responseServiceId] = std::move(callBackFunction);
  co_await controlSocket.writeTo(remoteControlEndPoint, request);
}

void IpKnxConnection::addListener(
    std::weak_ptr<KnxConnectionListener> listener) {
  connectionListeners.push_back(listener);
}

asio::awaitable<void> IpKnxConnection::start() {
  controlSocket.start();
  dataSocket.start();

  listeners.emplace(
      TunnelRequest::SERVICE_ID,
      std::bind_front(&IpKnxConnection::onReceiveTunnelRequest, this));
  listeners.emplace(
      DisconnectRequest::SERVICE_ID,
      std::bind_front(&IpKnxConnection::onReceiveDisconnectRequest, this));

  // send connect request
  auto controlHpai = createControlHPAI();
  auto dataHpai = createDataHPAI();
  auto cri = ConnectionRequestInformation::newTunneling();
  ConnectRequest connectRequest{std::move(controlHpai), std::move(dataHpai),
                                std::move(cri)};
  auto bytes = connectRequest.toBytes();
  co_await sendRequest(bytes, knx::requestresponse::ConnectResponse::SERVICE_ID,
                       [this](KnxIpHeader &, ByteBufferReader &data) {
                         const ConnectResponse connectResponse =
                             ConnectResponse::parse(data);
                         channelId = connectResponse.getChannelId();
                         asio::co_spawn(ctx, checkConnection(), asio::detached);
                         forEveryListener([](KnxConnectionListener *listener) {
                           listener->onConnect();
                         });
                         return false;
                       });
}

asio::awaitable<void> IpKnxConnection::checkConnection() {
  bool keepGoing = true;
  while (keepGoing) {
    checkConnectionTimer.expires_after(std::chrono::seconds(60));
    auto [errorcode] = co_await checkConnectionTimer.async_wait(asio::as_tuple);
    // if canceled, stop
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

auto IpKnxConnection::onReceiveTunnelRequest(KnxIpHeader &knxIpHeader,
                                             ByteBufferReader &reader) -> bool {
  // send a acknowlegde
  const TunnelRequest request = TunnelRequest::parse(reader);
  ConnectionHeader header{request.getConnectionHeader()};
  TunnelAckResponse response{std::move(header)};
  auto tunnelResponseBytes = response.toBytes();
  controlSocket.writeToSync(remoteControlEndPoint, tunnelResponseBytes);

  // send event to listeners
  const Cemi &cemi = request.getCemi();
  switch (request.getCemi().getNPDU().getACPI().getType()) {
  case DataACPI::GROUP_VALUE_READ:

    forEveryListener([cemi](KnxConnectionListener *listener) {
      listener->onGroupRead(cemi.getSource(),
                            std::get<GroupAddress>(cemi.getDestination()));
    });
    ;
    break;
  case DataACPI::GROUP_VALUE_RESPONSE:
    forEveryListener([cemi](KnxConnectionListener *listener) {
      listener->onGroupReadResponse(
          cemi.getSource(), std::get<GroupAddress>(cemi.getDestination()),
          cemi.getNPDU().getACPI().getData());
    });
    ;
    break;
  case DataACPI::GROUP_VALUE_WRITE:
    forEveryListener([cemi](KnxConnectionListener *listener) {
      listener->onGroupWrite(cemi.getSource(),
                             std::get<GroupAddress>(cemi.getDestination()),
                             cemi.getNPDU().getACPI().getData());
    });
    ;
    break;
  case DataACPI::INDIVIDUAL_ADDRESS_READ:
    break;
  case DataACPI::INDIVIDUAL_ADDRESS_WRITE:
    break;
  default:
    break;
  }
  return true;
}

auto IpKnxConnection::onReceiveDisconnectRequest(KnxIpHeader &knxIpHeader,
                                                 ByteBufferReader &reader)
    -> bool {
  const DisconnectRequest request = DisconnectRequest::parse(reader);
  if (request.getChannel() == channelId) {
    DisconnectResponse response{channelId};
    auto responseBytes = response.toBytes();
    controlSocket.writeToSync(remoteControlEndPoint, responseBytes);
    co_spawn(ctx, this->close(false), asio::detached);
  }
  return true;
}

void IpKnxConnection::onReceiveData(std::vector<std::uint8_t> &data) {
  ByteBufferReader reader(data);
  KnxIpHeader knxIpHeader = KnxIpHeader::createAndParse(reader);
  if (this->listeners.contains(knxIpHeader.getServiceType())) {
    const bool keep =
        this->listeners.at(knxIpHeader.getServiceType())(knxIpHeader, reader);
    if (!keep) {
      this->listeners.erase(knxIpHeader.getServiceType());
    }
  }
}

void IpKnxConnection::setGroupData(GroupAddress &ga, bool value) {
  // Contruct Cemi should be connection independend
  IndividualAddress source(1, 0, 3); // my address
  Control control{true};
  GroupAddress gaMove{ga};
  std::array<byte, 2> data{0x00};
  data[1] = value?0x01:0x00;
  DataACPI dataAcpi{DataACPI::GROUP_VALUE_WRITE, data};
  TCPI tcpi{false, false, 0x00};
  NPDUFrame npduFrame{std::move(tcpi), std::move(dataAcpi)};
  Cemi cemi{0x29, std::move(control), std::move(source),
            std::variant<IndividualAddress, GroupAddress>(gaMove),
            std::move(npduFrame)};

  // tunneling specific
  ConnectionHeader connectionHeader{channelId, 0x00, 0x00};
  TunnelRequest tunnelRequest{std::move(connectionHeader), std::move(cemi)};
  auto bytes = tunnelRequest.toBytes();

  co_spawn(ctx, controlSocket.writeTo(remoteControlEndPoint, bytes),
           asio::detached);
  // do something with response
}

bool IpKnxConnection::getGroupData(GroupAddress &ga) { return false; }

ConnectionRequestInformation
IpKnxConnection::createConnectRequestInformation() {
  return ConnectionRequestInformation::newTunneling();
}
HPAI IpKnxConnection::createDataHPAI() {
  return {IpAddress{this->localBindIp.to_bytes()}, this->dataPort, HPAI::UDP};
}

HPAI IpKnxConnection::createControlHPAI() {
  return {IpAddress{this->localBindIp.to_bytes()}, this->controlPort,
          HPAI::UDP};
};

void IpKnxConnection::forEveryListener(
    std::function<auto(KnxConnectionListener *)->void> doThis) {
  for (auto listenerRef : connectionListeners) {
    if (auto listener = listenerRef.lock()) {
      doThis(listener.get());
    }
  }
}

asio::awaitable<void>
IpKnxConnection::close(bool needsDisconnectRequest) {
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
            timer.cancel_one();
            return false;
          });
      auto [error] = co_await timer.async_wait(asio::as_tuple);
    }
    checkConnectionTimer.cancel_one();
    controlSocket.stop();
    dataSocket.stop();
    forEveryListener(
        [](KnxConnectionListener *listener) { listener->onDisconnect(); });
  }
}

} // namespace connection
