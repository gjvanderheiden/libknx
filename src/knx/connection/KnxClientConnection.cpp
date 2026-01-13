#include "knx/connection/KnxClientConnection.h"
#include "knx/cemi/ACPI.h"
#include "knx/cemi/Control.h"
#include "knx/connection/SendTunnelingState.h"
#include "knx/headers/KnxAddress.h"
#include <asio/awaitable.hpp>
#include <asio/steady_timer.hpp>
#include <functional>
#include <memory>

namespace knx::connection {

static std::uint32_t toMapKey(const Cemi &cemi, const GroupAddress &ga) {
  std::uint32_t key{0};
  std::uint8_t type = cemi.getNPDU().getACPI().getType();
  std::uint8_t high = ga.getHigh();
  std::uint8_t middle = ga.getMiddle();
  std::uint8_t low = ga.getLow();
  key = type;
  key |= high << 8;
  key |= middle << 16;
  key |= low << 24;
  return key;
}

using namespace std::chrono_literals;

KnxClientConnection::KnxClientConnection(
    asio::io_context &ctx,
    std::unique_ptr<TunnelingConnection> &&tunnelingConnection)
    : ctx{ctx}, tunnelingConnection{std::move(tunnelingConnection)} {
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

void KnxClientConnection::checkForConfirm(Cemi &cemi) {
  if (cemi.getMessageCode() != Cemi::L_DATA_CON)
    return;
  if (cemi.getSource() == this->tunnelingConnection->getKnxAddress()) {
    GroupAddress ga = std::get<GroupAddress>(cemi.getDestination());
    const auto key = toMapKey(cemi, ga);
    if (this->requests.contains(key)) {
      this->requests[key]->onResponse(true);
    }
  }
}

void KnxClientConnection::onIncommingCemi(Cemi &cemi) {
  checkForConfirm(cemi);
  checkForUpdateListener(cemi);
}

void KnxClientConnection::checkForUpdateListener(Cemi &cemi) {
  if (cemi.getMessageCode() != Cemi::L_DATA_IND)
    return;
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
}

asio::awaitable<void>
KnxClientConnection::writeToGroup(GroupAddress &ga, const std::uint8_t value,
                                  const bool only6Bits) {
  DataACPI dataAcpi{DataACPI::GROUP_VALUE_WRITE, value, only6Bits};

  co_await writeToGroup(ga, std::move(dataAcpi));
}

asio::awaitable<void>
KnxClientConnection::writeToGroup(GroupAddress &ga,
                                  const std::span<const std::uint8_t> value) {
  std::vector<byte> data;
  data.reserve(value.size());
  std::ranges::copy(value, std::back_inserter(data));
  DataACPI dataAcpi{DataACPI::GROUP_VALUE_WRITE, std::move(data)};
  co_await writeToGroup(ga, std::move(dataAcpi));
}

asio::awaitable<void> KnxClientConnection::sendCemi(Cemi &cemi) {
  auto dest = std::get<GroupAddress>(cemi.getDestination());
  const auto key = toMapKey(cemi, dest);
  this->requests[key] = std::make_unique<SendTunnelingState>(
      ctx, [&cemi, this]() -> asio::awaitable<void> {
        co_await tunnelingConnection->send(Cemi{cemi});
      });
  co_await this->requests[key]->send();
  std::cout << (this->requests[key]->getState() == SendTunnelingState::State::ok
                    ? "ok"
                    : "error")
            << " sending cemi.\n";
  this->requests.erase(key);
}

asio::awaitable<void> KnxClientConnection::writeToGroup(GroupAddress &ga,
                                                        DataACPI &&dataACPI) {
  Control control{KnxPrio::low, true};
  IndividualAddress source(0, 0, 0);
  TCPI tcpi{false, false, 0x00};
  NPDUFrame npduFrame{std::move(tcpi), std::move(dataACPI)};
  Cemi cemi{Cemi::L_DATA_REQ, std::move(control), std::move(source),
            std::variant<IndividualAddress, GroupAddress>(ga),
            std::move(npduFrame)};
  co_await sendCemi(cemi);
}

asio::awaitable<void>
KnxClientConnection::sendReadGroup(const GroupAddress &ga) {
  Control control{KnxPrio::low, true};
  IndividualAddress source(0, 0, 0);
  DataACPI dataAcpi{DataACPI::GROUP_VALUE_READ};
  TCPI tcpi{false, false, 0x00};
  NPDUFrame npduFrame{std::move(tcpi), std::move(dataAcpi)};
  Cemi cemi{Cemi::L_DATA_REQ, std::move(control), std::move(source),
            std::variant<IndividualAddress, GroupAddress>(ga),
            std::move(npduFrame)};
  co_await this->sendCemi(cemi);
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
