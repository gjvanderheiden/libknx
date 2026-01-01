#include "knx/connection/KnxClientConnection.h"
#include "knx/cemi/ACPI.h"
#include "knx/cemi/Control.h"
#include "knx/headers/KnxAddress.h"
#include <asio/as_tuple.hpp>
#include <asio/awaitable.hpp>
#include <asio/detached.hpp>
#include <asio/io_context.hpp>
#include <asio/ip/address_v4.hpp>
#include <asio/steady_timer.hpp>
#include <asio/use_awaitable.hpp>
#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>

namespace knx::connection {

using namespace std::chrono_literals;

KnxClientConnection::KnxClientConnection(
    std::unique_ptr<TunnelingConnection> &&tunnelingConnection)
    : tunnelingConnection{std::move(tunnelingConnection)} {
  this->tunnelingConnection->addListener(*this);
}

void KnxClientConnection::addListener(
    std::weak_ptr<KnxConnectionListener> listener) {
  connectionListeners.push_back(listener);
}

asio::awaitable<void> KnxClientConnection::start() {
  co_await tunnelingConnection->start();
}

asio::awaitable<void> KnxClientConnection::printDescription() {
  co_await tunnelingConnection->printDescription();
}

bool KnxClientConnection::isOpen() {
  return tunnelingConnection.get() != nullptr;
}

void KnxClientConnection::onIncommingCemi(Cemi &cemi) {
  if (cemi.getMessageCode() == Cemi::L_DATA_IND) {
    switch (cemi.getNPDU().getACPI().getType()) {
    case DataACPI::GROUP_VALUE_READ:
      forEveryListener([cemi](KnxConnectionListener *listener) {
        listener->onGroupRead(cemi.getSource(),
                              std::get<GroupAddress>(cemi.getDestination()));
      });
      ;
      break;
    case DataACPI::GROUP_VALUE_RESPONSE:
      forEveryListener([cemi](KnxConnectionListener *listener) {
        auto data = cemi.getNPDU().getACPI().getData();
        listener->onGroupReadResponse(
            cemi.getSource(), std::get<GroupAddress>(cemi.getDestination()),
            data);
      });
      ;
      break;
    case DataACPI::GROUP_VALUE_WRITE:
      forEveryListener([cemi](KnxConnectionListener *listener) {
        auto data = cemi.getNPDU().getACPI().getData();
        listener->onGroupWrite(cemi.getSource(),
                               std::get<GroupAddress>(cemi.getDestination()),
                               data);
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
  }
}

asio::awaitable<void> KnxClientConnection::writeToGroup(GroupAddress &ga,
                                                        std::uint8_t value,
                                                        bool only6Bits) {
  DataACPI dataAcpi{DataACPI::GROUP_VALUE_WRITE, value, only6Bits};
  co_await writeToGroup(ga, std::move(dataAcpi));
}

asio::awaitable<void>
KnxClientConnection::writeToGroup(GroupAddress &ga,
                                  std::span<const std::uint8_t> value) {
  DataACPI dataAcpi{DataACPI::GROUP_VALUE_WRITE, value};
  co_await writeToGroup(ga, std::move(dataAcpi));
}

asio::awaitable<void> KnxClientConnection::writeToGroup(GroupAddress &ga,
                                                        DataACPI &&dataACPI) {
  IndividualAddress source(0, 0, 0);
  Control control{KnxPrio::low, true};
  TCPI tcpi{false, false, 0x00};
  NPDUFrame npduFrame{std::move(tcpi), std::move(dataACPI)};
  Cemi cemi{Cemi::L_DATA_REQ, std::move(control), std::move(source),
            std::variant<IndividualAddress, GroupAddress>(ga),
            std::move(npduFrame)};
  co_await this->tunnelingConnection->send(std::move(cemi));
  // Server should send a Cemi:L_DATA_CON (confirmation)
}

asio::awaitable<void>
KnxClientConnection::sendReadGroup(const GroupAddress &ga) {
  IndividualAddress source(0, 0, 0);
  Control control{KnxPrio::low, true};
  DataACPI dataAcpi{DataACPI::GROUP_VALUE_READ};
  TCPI tcpi{false, false, 0x00};
  NPDUFrame npduFrame{std::move(tcpi), std::move(dataAcpi)};
  Cemi cemi{Cemi::L_DATA_REQ, std::move(control), std::move(source),
            std::variant<IndividualAddress, GroupAddress>(ga),
            std::move(npduFrame)};
  co_await this->tunnelingConnection->send(std::move(cemi));
  // group value response?
}

void KnxClientConnection::onConnect() {
  forEveryListener(
      [](KnxConnectionListener *listener) { listener->onConnect(); });
}

void KnxClientConnection::onDisconnect() {
  forEveryListener(
      [](KnxConnectionListener *listener) { listener->onDisconnect(); });
}

void KnxClientConnection::forEveryListener(
    std::function<auto(KnxConnectionListener *)->void> doThis) {
  for (auto listenerRef : connectionListeners) {
    if (auto listener = listenerRef.lock()) {
      doThis(listener.get());
    } else {
      std::cerr << "Lost a listener\n"; // prefectly normal actually, but need
                                        // to test this.
    }
  }
}

asio::awaitable<void> KnxClientConnection::close() {
  if (tunnelingConnection) {
    co_await this->tunnelingConnection->close();
    tunnelingConnection.reset();
  }
}

} // namespace knx::connection
