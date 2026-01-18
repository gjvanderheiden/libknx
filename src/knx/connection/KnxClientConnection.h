#pragma once

#include "knx/cemi/ACPI.h"
#include "knx/headers/KnxAddress.h"
#include "knx/connection/KnxConnectionListener.h"
#include "knx/connection/TunnelingConnection.h"
#include <asio/awaitable.hpp>
#include <asio/io_context.hpp>
#include <asio/steady_timer.hpp>
#include <cstdint>
#include <memory>
#include <vector>

namespace knx::connection {


/**
 * I represent a client connection to a knx system. For now I'm capable of using a
 * tunneling connection. I can receive group write messages, send group write
 * and send group reads.
 *
 * Detail about my resposiblities: I manage the communication of Cemi frames and
 * communicate with the tunnelingConnection.
 */
class KnxClientConnection final : ConnectionListener {
public:
  KnxClientConnection(asio::io_context& ctx, std::unique_ptr<TunnelingConnection> &&tunnelingConnection);
  ~KnxClientConnection() override = default;

  KnxClientConnection(KnxClientConnection&& other) noexcept = delete;
  KnxClientConnection& operator=(KnxClientConnection&& other) noexcept =  delete;

  /**
   * I will start up everything that is needed to maintain a connection with a
   * KNX IP Router.
   */
  [[nodiscard]] asio::awaitable<void> start() const;

  [[nodiscard]] bool isOpen() const;

  /**
   * Does not comply to RAII, but need to figure this one out a bit. Get the
   * project going.
   */
  [[nodiscard]] asio::awaitable<void> close();

  [[nodiscard]] asio::awaitable<void> printDescription() const;

  void addListener(std::weak_ptr<KnxConnectionListener> listener);

  /**
   * I'll send out a message to the IP KNX Router to send the value to the KNX
   * network
   */
  [[nodiscard]] asio::awaitable<void> writeToGroup(GroupAddress groupAddress, std::span<const std::uint8_t> value);

  /**
   * I'll send out a message to the IP KNX Router to send the value to the KNX
   * network
   */
  asio::awaitable<void> writeToGroup(GroupAddress groupAddress, std::uint8_t value, bool only6Bits = false);

  /**
   * I will send out a request to read, if a KNX device responds,
   * it'll trigger a onGroupReadResponse on the listeners (KnxConnectionListener).
   * see addListener().
   */
  asio::awaitable<void> sendReadGroup(const GroupAddress groupAddress);

private:
  void
  forEveryListener(const std::function<auto(KnxConnectionListener *)->void>& doThis);

  void onConnect() override;
  void onDisconnect() override;
  void onIncommingCemi(Cemi &cemi) override;
  void checkForUpdateListener(Cemi &cemi);
  void checkForConfirm(Cemi& cemi);

  asio::awaitable<void> writeToGroup(GroupAddress groupAddress, DataACPI&& dataACPI);
  asio::awaitable<void> sendCemi(Cemi& cemi);

private:
  asio::io_context& ctx;
  std::unique_ptr<TunnelingConnection> tunnelingConnection;
  std::vector<std::weak_ptr<KnxConnectionListener>> connectionListeners;
  std::map<std::uint32_t, std::unique_ptr<SendTunnelingState>> requests;
};

} // namespace connection
