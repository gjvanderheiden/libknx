#pragma once

#include "knx/headers/ConnectionHeader.h"
#include <asio/awaitable.hpp>
#include <asio/io_context.hpp>
#include <asio/steady_timer.hpp>

namespace knx::connection {

/**
 *
 */
class SendTunnelingState {
public:
  enum class State : std::uint8_t {
    initial,
    ok,
    sending,
    timeout,
    canceled,
    error
  };

  using SendMethod = std::function<asio::awaitable<void>()>;

public:
  SendTunnelingState(asio::io_context &ctx, SendMethod &&sendMethod);

  [[nodiscard]] State getState() const;
  void onReceiveAckTunnelResponse(const ConnectionHeader &connectionHeader);
  void onResponse(bool okStatus);
  asio::awaitable<void> send(unsigned int attempts = 2);
  void cancel();

private:
  State state{State::initial};
  asio::steady_timer sendTimer;
  SendMethod sendMethod;
};

} // namespace knx::connection
