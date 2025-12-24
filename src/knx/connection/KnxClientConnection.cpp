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
#include <memory>
#include <iostream>

namespace knx::connection {

using namespace std::chrono_literals;

KnxClientConnection::KnxClientConnection(
    std::unique_ptr<TunnelingConnection> &&tunnelingConnection)
    : tunnelingConnection{std::move(tunnelingConnection)} {
    }


void KnxClientConnection::addListener(
    std::weak_ptr<KnxConnectionListener> listener) {
  connectionListeners.push_back(listener);
}

asio::awaitable<void> KnxClientConnection::start() {
  tunnelingConnection->addListener(*this);
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

Cemi createGroupWriteCemi(const GroupAddress &ga, const KnxPrio prio,
                          std::vector<std::uint8_t> &&data) {
  IndividualAddress source(0, 0, 0);
  Control control{prio, true};
  DataACPI dataAcpi{DataACPI::GROUP_VALUE_WRITE, std::move(data)};
  TCPI tcpi{false, false, 0x00};
  NPDUFrame npduFrame{std::move(tcpi), std::move(dataAcpi)};
  return Cemi{Cemi::L_DATA_REQ, std::move(control), std::move(source),
              std::variant<IndividualAddress, GroupAddress>(ga),
              std::move(npduFrame)};
}

asio::awaitable<void> KnxClientConnection::writeToGroup(GroupAddress &ga,
                                       std::uint8_t value, bool only6Bits) {
  IndividualAddress source(0, 0, 0);
  Control control{KnxPrio::low, true};
  DataACPI dataAcpi{DataACPI::GROUP_VALUE_WRITE, value, only6Bits};
  TCPI tcpi{false, false, 0x00};
  NPDUFrame npduFrame{std::move(tcpi), std::move(dataAcpi)};
  Cemi cemi{Cemi::L_DATA_REQ, std::move(control), std::move(source),
            std::variant<IndividualAddress, GroupAddress>(ga),
            std::move(npduFrame)};
  co_await this->tunnelingConnection->send(std::move(cemi));
  /*
  std::shared_ptr timer = std::make_shared<asio::steady_timer>(ctx);
  this->waitingAcks[ga] = timer;
  timer->expires_after(50ms);
  auto [errorcode] = co_await timer->async_wait(asio::as_tuple);
  if(!errorcode) {
    std::cout << "o oh\n";
  }
  this->waitingAcks.erase(ga);
  */
  
  // Server should send a Cemi:L_DATA_CON (confirmation)
}
asio::awaitable<void> KnxClientConnection::writeToGroup(GroupAddress &ga,
                                       std::span<const std::uint8_t> value) {
  IndividualAddress source(0, 0, 0);
  Control control{KnxPrio::low, true};
  DataACPI dataAcpi{DataACPI::GROUP_VALUE_WRITE, value};
  TCPI tcpi{false, false, 0x00};
  NPDUFrame npduFrame{std::move(tcpi), std::move(dataAcpi)};
  Cemi cemi{Cemi::L_DATA_REQ, std::move(control), std::move(source),
            std::variant<IndividualAddress, GroupAddress>(ga),
            std::move(npduFrame)};
  co_await this->tunnelingConnection->send(std::move(cemi));
  /*
  std::shared_ptr timer = std::make_shared<asio::steady_timer>(ctx);
  this->waitingAcks[ga] = timer;
  timer->expires_after(50ms);
  auto [errorcode] = co_await timer->async_wait(asio::as_tuple);
  if(!errorcode) {
    std::cout << "o oh\n";
  }
  this->waitingAcks.erase(ga);
  */
  
  // Server should send a Cemi:L_DATA_CON (confirmation)
}

asio::awaitable<void> KnxClientConnection::readGroup(const GroupAddress &ga) {
  co_await sendReadGroup(ga);
}

asio::awaitable<void> KnxClientConnection::sendReadGroup(const GroupAddress &ga) {
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
  this->tunnelingConnection.reset();
}

void KnxClientConnection::forEveryListener(
    std::function<auto(KnxConnectionListener *)->void> doThis) {
  for (auto listenerRef : connectionListeners) {
    if (auto listener = listenerRef.lock()) {
      doThis(listener.get());
    } else {
      std::cerr << "Lost a listener\n"; // prefectly normal actually, but need to test this.
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
