#pragma once

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
 * Detail about my resposiblities: I manage the communicition of Cemi frames and
 * communicate with the tunnelingConnection.
 */
class KnxClientConnection final : ConnectionListener {
public:
  KnxClientConnection(std::unique_ptr<TunnelingConnection> &&tunnelingConnection);
  ~KnxClientConnection() override = default;
  KnxClientConnection(KnxClientConnection&& other) noexcept = default;
  KnxClientConnection& operator=(KnxClientConnection&& other) noexcept =  default;

public:
  /**
   * I will start up everything that is needed to maintain a connection with a
   * KNX IP Router.
   */
  asio::awaitable<void> start();
  bool isOpen();
  asio::awaitable<void> printDescription();

  void addListener(std::weak_ptr<KnxConnectionListener> listener);

  /**
   * I'll send out a message to the IP KNX Router to send the value to the KNX
   * network
   */
  asio::awaitable<void> writeToGroup(GroupAddress &ga, std::span<const std::uint8_t> value);

  /**
   * I'll send out a message to the IP KNX Router to send the value to the KNX
   * network
   */
  asio::awaitable<void> writeToGroup(GroupAddress &ga, std::uint8_t value, bool only6Bits = false);

  /**
   * Get the data of the specified group address
   */
  asio::awaitable<void> readGroup(const GroupAddress &ga);

  /**
   * I will send out a request to read, if a KNX device responds,
   * it'll trigger a onGroupReadResponse on the listeners (KnxConnectionListener).
   * see addListener().
   */
  asio::awaitable<void> sendReadGroup(const GroupAddress &ga);

  /**
   * Does not comply to RAII, but need to figure this one out a bit. Get the
   * project going.
   */
  asio::awaitable<void> close();

private:
  void
  forEveryListener(std::function<auto(KnxConnectionListener *)->void> doThis);

  void onConnect() override;
  void onDisconnect() override;
  void onIncommingCemi(Cemi &cemi) override;

private:
  std::unique_ptr<TunnelingConnection> tunnelingConnection;
  std::vector<std::weak_ptr<KnxConnectionListener>> connectionListeners{};
  friend class Listener;
};

} // namespace connection
