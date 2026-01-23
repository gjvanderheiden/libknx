#include "knx/connection/TunnelingConnection.h"
#include "knx/bytes/ByteBufferReader.h"
#include "knx/headers/ConnectionRequestInformation.h"
#include "knx/headers/KnxAddress.h"
#include "knx/ip/UdpSocket.h"
#include "knx/ipreqresp/KnxIpHeader.h"
#include "knx/ipreqresp/RequestResponseFactory.h"
#include "knx/ipreqresp/requests//DisconnectRequest.h"
#include "knx/ipreqresp/requests/ConnectRequest.h"
#include "knx/ipreqresp/requests/ConnectStateRequest.h"
#include "knx/ipreqresp/requests/DescriptionRequest.h"
#include "knx/ipreqresp/requests/TunnelingRequest.h"
#include "knx/ipreqresp/responses/ConnectResponse.h"
#include "knx/ipreqresp/responses/ConnectStateResponse.h"
#include "knx/ipreqresp/responses/DescriptionResponse.h"
#include "knx/ipreqresp/responses/DisconnectResponse.h"
#include "knx/ipreqresp/responses/TunnelAckResponse.h"
#include "knx/ipreqresp/responses/responses.h"
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
#include <memory>
#include <variant>

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
}

TunnelingConnection::~TunnelingConnection() { closeWithoutDisconnect(); }

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
  auto response = co_await sendRequest(connectRequest);
  if (std::holds_alternative<ConnectResponse>(response)) {
    std::cout << "got a connect response\n";
    const ConnectResponse connectResponse = std::get<ConnectResponse>(response);
    channelId = connectResponse.getChannelId();
    auto remoteIp = connectResponse.getDataEndPoint().getAddress().asString();
    auto port = connectResponse.getDataEndPoint().getPort();
    remoteDataEndPoint = {asio::ip::make_address_v4(remoteIp), port};
    asio::co_spawn(ctx, checkConnection(), asio::detached);
    knxAddress = connectResponse.getCRD().getAddress();
    forEveryListener(
        [](ConnectionListener *listener) { listener->onConnect(); });
    std::cout << "Tunneling started\n";
  } else {
    reset();
  }
}

asio::awaitable<void> TunnelingConnection::checkConnection() {
  bool keepGoing = true;
  while (keepGoing) {
    checkConnectionTimer.expires_after(
        std::chrono::seconds(TIME_BETWEEN_CHECK_CONNECTION));
    auto [errorcode] = co_await checkConnectionTimer.async_wait(asio::as_tuple);
    // if cancelled, stop
    if (errorcode) {
      keepGoing = false;
      break;
    }
    // send a connect state request
    std::cout << "Sending connect state request\n";
    ConnectStateRequest request{createControlHPAI(), channelId};
    auto response = co_await sendRequest(request);
    if (std::holds_alternative<TunnelAckResponse>(response)) {
      ConnectStateResponse connectStateResponse =
          std::get<ConnectStateResponse>(response);
      if (connectStateResponse.getStatus() != 0x00) {
        std::cout << "Got an error ("
                  << static_cast<int>(connectStateResponse.getStatus())
                  << ") closing.\n";
        co_spawn(ctx, this->close(), asio::detached);
        keepGoing = false;
      }
    } else {
      std::cout << "Got no response to Connect state request\n";
      // co_spawn(ctx, this->close(), asio::detached);
      keepGoing = false;
    }
  }
}

void TunnelingConnection::onReceiveTunnelRequest(const TunnelRequest & request) {
  // send an acknowlegde
  {
    TunnelAckResponse response{ConnectionHeader{request.getConnectionHeader()}};
    auto tunnelResponseBytes = response.toBytes();
    controlSocket->writeToSync(remoteControlEndPoint, tunnelResponseBytes);
  }

  auto cemi = request.getCemi();
  forEveryListener([&cemi](ConnectionListener *listener) {
    listener->onIncommingCemi(cemi);
  });

}

void TunnelingConnection::onReceiveDisconnectRequest(
    const DisconnectRequest &request) {
  std::cout << "Received disconnect request\n";
  if (request.getChannel() == channelId) {
    DisconnectResponse response{channelId};
    auto responseBytes = response.toBytes();
    controlSocket->writeToSync(remoteControlEndPoint, responseBytes);
    this->reset();

    forEveryListener(
        [](ConnectionListener *listener) { listener->onDisconnect(); });
  }
}

void TunnelingConnection::onReceiveData(ByteSpan data) {
  std::cout << "TunnelingConnection::onReceiveData(std::vector<std::uint8_t> "
               "&&data)\n";
  ByteBufferReader reader(data);
  KnxIpHeader knxIpHeader = KnxIpHeader::parse(reader);
  std::cout << "TunnelingConnection::onReceiveData() : service type =  "
    << std::hex << knxIpHeader.getServiceType() << "\n";


  auto type = RequestResponseFactory::getType(knxIpHeader);
  if (type == MessageType::unknown) {
    return;
  }
  if (type == MessageType::response) {
    ResponseVariant response =
        RequestResponseFactory::parseResponse(knxIpHeader, reader);
    std::string description = std::visit(
        Overloaded{[](const std::monostate & /*monostate*/) -> std::string {
                     return "nothing";
                   },
                   [](const auto &response) {
                     return response.getShortDescription();
                   }},
        response);

    std::cout << "Received: " << description << "\n";
    bool foundMatch = false;
    for (auto const &sendRequest : this->sendItems) {
      if (sendRequest->matchResponse(std::move(response))) {
        foundMatch = true;
        std::cout << "found match for " << std::hex
                  << knxIpHeader.getServiceType() << "\n";
        break;
      }
    }
    if (!foundMatch) {
      std::cout << "NO MATCH found for " << std::hex
                << knxIpHeader.getServiceType() << "\n";
    }
  } else {
    RequestVariant request =
        RequestResponseFactory::parseRequest(knxIpHeader, reader);

    std::string description = std::visit(
        Overloaded{
            [](const std::monostate & /*monostate*/) -> std::string {
              return "nothing";
            },
            [](const auto &request) { return request.getShortDescription(); }},
        request);
    std::cout << "Received: " << description << "\n";

    std::visit(Overloaded{[this](const TunnelRequest &tunnelRequest) {onReceiveTunnelRequest(tunnelRequest);},
                          [this](const DisconnectRequest &request) {onReceiveDisconnectRequest(request);},
                          [](const std::monostate & /*monostate*/) {},
                          [](const auto &request) {std::cout << "No action for " << request.getShortDescription() <<"\n";}},
               request);
  }
}

asio::awaitable<void> TunnelingConnection::send(Cemi cemi) {
  std::cout << "TunnelingConnection::send(Cemi)\n";
  std::uint8_t sequenceSend = sequence++;
  const TunnelRequest tunnelRequest{ConnectionHeader{channelId, sequenceSend},
                                    std::move(cemi)};
  auto response = co_await sendRequest(tunnelRequest);
  if (std::holds_alternative<TunnelAckResponse>(response)) {
    auto ackResponse = std::get<TunnelAckResponse>(response);
    if (ackResponse.getConnectionHeader().getStatus() != 0x00) {
      std::cout << "Got unexpected ack status " << std::hex
                << ackResponse.getConnectionHeader().getStatus() << "\n";
      co_await this->close();
    }
  } else {
    std::cout << "Received unexpected response\n";
  }
}

asio::awaitable<ResponseVariant>
TunnelingConnection::sendRequest(const AbstractRequest &request) {
  const auto matcher = [&request](ResponseVariant &response) -> bool {
    return request.matchesResponse(response);
  };

  auto sendItemUP = std::make_unique<TunnelingSendState>(
      ctx,
      [this, &request]() -> asio::awaitable<void> {
        std::cout << "Sending request: " << request.getShortDescription()
                  << "\n";
        std::vector<byte> data = request.toBytes();
        if (request.getServiceType() == TunnelRequest::SERVICE_ID) {
          co_await dataSocket->writeTo(this->remoteDataEndPoint, data);
        } else {
          co_await controlSocket->writeTo(this->remoteControlEndPoint, data);
        }
      },
      matcher);
  auto *item = sendItemUP.get();
  sendItems.push_back(std::move(sendItemUP));

  std::cout << "Awaiting sending request: " << request.getShortDescription()
                  << "\n";
  co_await item->send();
  bool timedOut = item->getState() == TunnelingSendState::State::timeout;
  if (timedOut) {
    std::cerr << "Timeout for " << std::hex << request.getServiceType()
              << std::dec << ", " << request.getShortDescription() << "\n";
    co_await close();
  }
  auto answer = item->getResponse();
  std::erase_if(sendItems,
                [&item](const auto &element) { return element.get() == item; });
  co_return answer;
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
  auto response = co_await sendRequest(request);
  if (std::holds_alternative<DescriptionResponse>(response)) {
    const auto descriptionResponse = std::get<DescriptionResponse>(response);
    std::cout << "supported service families ("
              << descriptionResponse.getSupportedServiceFamiliesDib()
                     .getServiceFamilies()
                     .size()
              << "):\n";
    for (auto fam : descriptionResponse.getSupportedServiceFamiliesDib()
                        .getServiceFamilies()) {
      std::cout << fam.toString() << '\n';
    }
  }
}

void TunnelingConnection::onControlSocketClosed() {
  std::cerr << "control socket closed\n";
  closeWithoutDisconnect();
}

void TunnelingConnection::onDataSocketClosed() {
  std::cerr << "data socket closed\n";
  closeWithoutDisconnect();
}

const IndividualAddress &TunnelingConnection::getKnxAddress() {
  return knxAddress;
}

asio::awaitable<void> TunnelingConnection::close() {
  if (!closingDown) {
    closingDown = true;
    DisconnectRequest disconnectRequest{channelId, createControlHPAI()};
    auto response = co_await sendRequest(disconnectRequest);
    if (std::holds_alternative<DisconnectResponse>(response)) {
      std::cout << "Server responded to closing tunnel request\n";
    } else {
      std::cerr << "Server didn't responded to closing tunnel request, just "
                   "closing now\n";
    }
    closeWithoutDisconnect();
    closingDown = false;
  }
}

void TunnelingConnection::closeWithoutDisconnect() {
  this->reset();
  if (controlSocket) {
    controlSocket->stop();
  }
  if (dataSocket) {
    dataSocket->stop();
  }
  forEveryListener(
      [](ConnectionListener *listener) { listener->onDisconnect(); });
}

void TunnelingConnection::reset() {
  checkConnectionTimer.cancel();
  for (const auto &sendTunnelingState : this->sendItems) {
    sendTunnelingState->cancel();
  }
  this->sendItems.clear();
  this->sequence = 0;
  this->knxAddress = {0, 0, 0};
}

} // namespace knx::connection
