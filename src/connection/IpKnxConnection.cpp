
#include "IpKnxConnection.h"
#include "ByteBufferReader.h"
#include "ConnectRequest.h"
#include "ConnectionRequestInformation.h"
#include "KnxIpHeader.h"
#include "TunnelingRequest.h"
#include "requests/ConnectStateRequest.h"
#include "requests/DisconnectRequest.h"
#include "responses/ConnectResponse.h"
#include "responses/ConnectStateResponse.h"
#include "responses/DisconnectResponse.h"
#include "responses/TunnelAckResponse.h"
#include <asio/awaitable.hpp>
#include <asio/detached.hpp>
#include <asio/io_context.hpp>
#include <asio/ip/address_v4.hpp>
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
    : ctx{ctx},
      remoteControlEndPoint{asio::ip::make_address_v4(remoteIp), remotePort},
      localBindIp{asio::ip::make_address_v4(localBindIp)},
      dataPort{localDataPort}, controlPort{localControlPort},
      dataSocket{ctx, localBindIp, localDataPort},
      controlSocket{ctx, localBindIp, localControlPort} {
  dataSocket.setHandler(std::bind_front(&IpKnxConnection::onReceiveData, this));
  controlSocket.setHandler(
      std::bind_front(&IpKnxConnection::onReceiveData, this));
}

asio::awaitable<void>
IpKnxConnection::sendRequest(std::vector<std::uint8_t> &request,
                             const std::uint16_t responseServiceId,
                             CallBackFunction &&callBackFunction) {
  listeners[responseServiceId] = std::move(callBackFunction);
  co_await controlSocket.writeTo(remoteControlEndPoint, request);
}

asio::awaitable<void> IpKnxConnection::start() {
  controlSocket.start();
  dataSocket.start();

  auto controlHpai = createControlHPAI();
  auto dataHpai = createDataHPAI();
  auto cri = ConnectionRequestInformation::defaultTunnelingCRI();
  ConnectRequest connectRequest{std::move(controlHpai), std::move(dataHpai),
                                std::move(cri)};
  auto bytes = connectRequest.toBytes();
  listeners.emplace(
      TunnelRequest::SERVICE_ID,
      std::bind_front(&IpKnxConnection::onReceiveTunnelRequest, this));
  listeners.emplace(
      DisconnectRequest::SERVICE_ID,
      std::bind_front(&IpKnxConnection::onReceiveDisconnectRequest, this));
  co_await sendRequest(bytes, knx::requestresponse::ConnectResponse::SERVICE_ID,
                       [this](KnxIpHeader &, ByteBufferReader &data) {
                         const ConnectResponse connectResponse =
                             ConnectResponse::parse(data);
                         std::cout << "connected\n";
                         channelId = connectResponse.getChannelId();
                         asio::co_spawn(ctx, checkConnection(), asio::detached);
                         return false;
                       });
}

asio::awaitable<void> IpKnxConnection::checkConnection() {
  using namespace std::literals;
  asio::steady_timer timer(co_await asio::this_coro::executor);
  bool keepGoing = true;
  while (keepGoing) {
    timer.expires_after(std::chrono::seconds(60));
    co_await timer.async_wait();
    ConnectStateRequest request{createControlHPAI(), channelId};
    auto bytes = request.toBytes();
    co_await sendRequest(
        bytes, knx::requestresponse::ConnectStateResponse::SERVICE_ID,
        [this, &keepGoing](KnxIpHeader &, ByteBufferReader &data) {
          ConnectStateResponse response = ConnectStateResponse::parse(data);
          if (response.getStatus()) {
            std::cout << "Got an error (" << (int)response.getStatus()
                      << ") closing.\n";
            this->close();
            keepGoing = false;
          }
          return false;
        });
  }
}

auto IpKnxConnection::onReceiveTunnelRequest(KnxIpHeader &knxIpHeader,
                                             ByteBufferReader &reader) -> bool {
  const TunnelRequest request = TunnelRequest::parse(reader);
  ConnectionHeader header{request.getConnectionHeader()};
  std::cout << "Got a TunnelRequest with sequence: "
            << (int)header.getSeqeunce() << std::endl;
  TunnelAckResponse response{std::move(header)};
  auto tunnelResponseBytes = response.toBytes();
  controlSocket.writeToSync(remoteControlEndPoint, tunnelResponseBytes);
  switch(request.getCemi().getNPDU().getACPI().getType()) {
    case DataACPI::GROUP_VALUE_READ:
      std::cout << "Group value read\n";
      break;
    case DataACPI::GROUP_VALUE_RESPONSE:
      std::cout << "Group value response\n";
      break;
    case DataACPI::GROUP_VALUE_WRITE:
      std::cout << "Group value write\n";
      break;
    case DataACPI::INDIVIDUAL_ADDRESS_READ:
      std::cout << "Individual address read\n";
      break;
    case DataACPI::INDIVIDUAL_ADDRESS_WRITE:
      std::cout << "Group value write\n";
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
    this->close();
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

void IpKnxConnection::setGroupData(GroupAddress &ga, bool value) {}

bool IpKnxConnection::getGroupData(GroupAddress &ga) { return false; }

ConnectionRequestInformation
IpKnxConnection::createConnectRequestInformation() {
  return ConnectionRequestInformation::defaultTunnelingCRI();
}
HPAI IpKnxConnection::createDataHPAI() {
  return {IpAddress{this->localBindIp.to_bytes()}, this->dataPort, HPAI::UDP};
}

HPAI IpKnxConnection::createControlHPAI() {
  return {IpAddress{this->localBindIp.to_bytes()}, this->controlPort,
          HPAI::UDP};
};

void IpKnxConnection::close() {
  // not nice, but it works for now
  ctx.stop();
  // send DisconnectRequest if possible
  // close udp's
}

} // namespace connection
