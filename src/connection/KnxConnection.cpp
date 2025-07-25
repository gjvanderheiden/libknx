#include "KnxConnection.h"
#include "KnxAddress.h"
#include "cemi/ACPI.h"
#include "cemi/Control.h"
#include <asio/as_tuple.hpp>
#include <asio/awaitable.hpp>
#include <asio/detached.hpp>
#include <asio/io_context.hpp>
#include <asio/ip/address_v4.hpp>
#include <asio/use_awaitable.hpp>
#include <cstdint>
#include <functional>
#include <memory>

namespace connection {

KnxConnection::KnxConnection(asio::io_context &ctx,
    std::unique_ptr<TunnelingConnection>&& tunnelingConnection) 
    : ctx{ctx}, tunnelingConnection{std::move(tunnelingConnection)} {}

KnxConnection::~KnxConnection() { std::cout << "~IpKnxConnection()\n"; }

void KnxConnection::addListener(
    std::weak_ptr<KnxConnectionListener> listener) {
  connectionListeners.push_back(listener);
}

asio::awaitable<void> KnxConnection::start() {
  tunnelingConnection->addListener(*this);
  co_await tunnelingConnection->start();
}

void KnxConnection::onIncommingCemi(Cemi &cemi) {
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
  }
}

Cemi createGroupWriteCemi(GroupAddress &ga, KnxPrio prio,
                          std::array<std::uint8_t, 2> &&data) {
  IndividualAddress source(0, 0, 0);
  Control control{KnxPrio::low, true};
  DataACPI dataAcpi{DataACPI::GROUP_VALUE_WRITE, data};
  TCPI tcpi{false, false, 0x00};
  NPDUFrame npduFrame{std::move(tcpi), std::move(dataAcpi)};
  return Cemi{Cemi::L_DATA_REQ, std::move(control), std::move(source),
              std::variant<IndividualAddress, GroupAddress>(ga),
              std::move(npduFrame)};
}

void KnxConnection::writeToGroup(GroupAddress &ga,
                                   std::array<std::uint8_t, 2> value) {
  IndividualAddress source(0, 0, 0);
  Control control{KnxPrio::low, true};
  DataACPI dataAcpi{DataACPI::GROUP_VALUE_WRITE, value};
  TCPI tcpi{false, false, 0x00};
  NPDUFrame npduFrame{std::move(tcpi), std::move(dataAcpi)};
  Cemi cemi{Cemi::L_DATA_REQ, std::move(control), std::move(source),
              std::variant<IndividualAddress, GroupAddress>(ga),
              std::move(npduFrame)};
  this->tunnelingConnection->send(std::move(cemi));
  // Server should send a Cemi:L_DATA_CON (confirmation)
}

std::array<std::uint8_t, 2> KnxConnection::readGroup(GroupAddress &ga) {
  return {0};
}

void KnxConnection::onConnect() {
  forEveryListener(
      [](KnxConnectionListener *listener) { listener->onConnect(); });
}

void KnxConnection::onDisconnect() {
  forEveryListener(
      [](KnxConnectionListener *listener) { listener->onDisconnect(); });
  this->tunnelingConnection.reset();
}

void KnxConnection::forEveryListener(
    std::function<auto(KnxConnectionListener *)->void> doThis) {
  for (auto listenerRef : connectionListeners) {
    if (auto listener = listenerRef.lock()) {
      doThis(listener.get());
    }
  }
}

asio::awaitable<void> KnxConnection::close() {
  return this->tunnelingConnection->close();
}

} // namespace connection
