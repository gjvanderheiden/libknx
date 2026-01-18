#include "knx/connection/SendTunnelingState.h"
#include <asio/as_tuple.hpp>
#include <asio/awaitable.hpp>
#include <asio/detached.hpp>
#include <asio/error.hpp>
#include <asio/io_context.hpp>
#include <asio/steady_timer.hpp>

namespace knx::connection {

using namespace std::chrono_literals;

SendTunnelingState::SendTunnelingState(asio::io_context &ctx,
                                       SendMethod &&sendMethod)

    : sendTimer{ctx}, sendMethod{std::move(sendMethod)} {}

void SendTunnelingState::onReceiveAckTunnelResponse(
    const ConnectionHeader &connectionHeader) {
  this->onResponse(connectionHeader.getStatus() == 0x00);
}

void SendTunnelingState::onResponse(bool okStatus) {
  if (okStatus) {
    state = State::ok;
  } else {
    state = State::error;
  }
  sendTimer.cancel();
}

SendTunnelingState::State SendTunnelingState::getState() const { return state; }

void SendTunnelingState::cancel() {
  state = State::canceled;
  sendTimer.cancel();
}

asio::awaitable<void> SendTunnelingState::send(const unsigned int attempts) {
  state = State::sending;
  co_await sendMethod();
  sendTimer.expires_after(50ms);
  auto [error] = co_await sendTimer.async_wait(asio::as_tuple);
  if (!error) {
    if (attempts > 1) {
      co_await send(attempts - 1);
    } else {
      state = State::timeout;
    }
  } else if (error != asio::error::operation_aborted) {
    state = State::error;
  }
}

} // namespace knx::connection
