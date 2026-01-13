#include "knx/connection/KnxClientConnection.h"
#include "knx/cemi/ACPI.h"
#include "knx/cemi/Control.h"
#include "knx/headers/KnxAddress.h"
#include <asio/awaitable.hpp>
#include <asio/steady_timer.hpp>
#include <functional>
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
  connectionListeners.push_back(std::move(listener));
}

asio::awaitable<void> KnxClientConnection::start() const {
  co_await tunnelingConnection->start();
}

asio::awaitable<void> KnxClientConnection::printDescription() const {
  co_await tunnelingConnection->printDescription();
}

bool KnxClientConnection::isOpen() const {
  return tunnelingConnection != nullptr;
}

void KnxClientConnection::onIncommingCemi(Cemi &cemi) {
  if (cemi.getMessageCode() == Cemi::L_DATA_IND) {
    switch (cemi.getNPDU().getACPI().getType()) {
    case DataACPI::GROUP_VALUE_READ:
      forEveryListener([cemi](KnxConnectionListener *listener) {
        listener->onGroupRead(cemi.getSource(),
                              std::get<GroupAddress>(cemi.getDestination()));
      });
      break;
    case DataACPI::GROUP_VALUE_RESPONSE: 
      forEveryListener([cemi](KnxConnectionListener *listener) {
          auto data = cemi.getNPDU().getACPI().getData();
        listener->onGroupReadResponse(
            cemi.getSource(), std::get<GroupAddress>(cemi.getDestination()),
            data);
      });
      break;
    case DataACPI::GROUP_VALUE_WRITE:
      forEveryListener([cemi](KnxConnectionListener *listener) {
        auto data = cemi.getNPDU().getACPI().getData();
        listener->onGroupWrite(cemi.getSource(),
                               std::get<GroupAddress>(cemi.getDestination()),
                               data);
      });
      break;
    case DataACPI::INDIVIDUAL_ADDRESS_READ:
      break;
    case DataACPI::INDIVIDUAL_ADDRESS_WRITE:
      break;
    default:
      break;
    }
  } else if(cemi.getMessageCode() == Cemi::L_DATA_CON) {
  }
}

asio::awaitable<void>
KnxClientConnection::writeToGroup(GroupAddress &ga, const std::uint8_t value,
                                  const bool only6Bits) const {
  DataACPI dataAcpi{DataACPI::GROUP_VALUE_WRITE, value, only6Bits};
  co_await writeToGroup(ga, std::move(dataAcpi));
}

asio::awaitable<void> KnxClientConnection::writeToGroup(
    GroupAddress &ga, const std::span<const std::uint8_t> value) const {
  std::vector<byte> data;
  data.reserve(value.size());
  std::ranges::copy(value, std::back_inserter(data));
  DataACPI dataAcpi{DataACPI::GROUP_VALUE_WRITE, std::move(data)};
  co_await writeToGroup(ga, std::move(dataAcpi));
}

asio::awaitable<void>
KnxClientConnection::writeToGroup(GroupAddress &ga, DataACPI &&dataACPI) const {
  Control control{KnxPrio::low, true};
  IndividualAddress source(0, 0, 0);
  TCPI tcpi{false, false, 0x00};
  NPDUFrame npduFrame{std::move(tcpi), std::move(dataACPI)};
  Cemi cemi{Cemi::L_DATA_REQ, std::move(control), std::move(source),
            std::variant<IndividualAddress, GroupAddress>(ga),
            std::move(npduFrame)};
  co_await this->tunnelingConnection->send(std::move(cemi));
  // Server should send a Cemi:L_DATA_CON (confirmation)
}

asio::awaitable<void>
KnxClientConnection::sendReadGroup(const GroupAddress &ga) const {
  Control control{KnxPrio::low, true};
  IndividualAddress source(0, 0, 0);
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
    std::function<auto(KnxConnectionListener *)->void> &&doThis) {
  bool hasLoosListener = false;
  for (const auto &listenerRef : connectionListeners) {
    if (auto listener = listenerRef.lock()) {
      doThis(listener.get());
    } else {
      hasLoosListener = true;
    }
  }
  if (hasLoosListener) {
    std::erase_if(connectionListeners,
                  [](auto &listenerRef) { return listenerRef.expired(); });
  }
}

asio::awaitable<void> KnxClientConnection::close() {
  if (tunnelingConnection) {
    co_await this->tunnelingConnection->close();
    tunnelingConnection.reset();
  }
}

} // namespace knx::connection
