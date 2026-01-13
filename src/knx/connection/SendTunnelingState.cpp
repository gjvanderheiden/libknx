#include "knx/connection/SendTunnelingState.h"
#include "knx/requests/TunnelingRequest.h"
#include <asio/as_tuple.hpp>
#include <asio/awaitable.hpp>
#include <asio/detached.hpp>
#include <asio/error.hpp>
#include <asio/io_context.hpp>
#include <asio/ip/address_v4.hpp>
#include <asio/steady_timer.hpp>

namespace knx::connection {

using namespace std::chrono_literals;

SendTunnelingState::SendTunnelingState(Cemi &&cemi, std::uint8_t channelId,
                                       std::uint8_t sequence,
                                       asio::io_context &ctx,
                                       SendMethod&& sendMethod)
    : sequence{sequence},
      sendTimer{ctx}, sendMethod{std::move(sendMethod)}, data{TunnelRequest{{channelId, sequence, 0x00}, std::move(cemi)}
        .toBytes()} {}

void SendTunnelingState::onReceiveAckTunnelResponse(
    const ConnectionHeader &connectionHeader) {
  if(connectionHeader.getStatus()) {
    state = State::error;
  } else {
    state = State::ok;
  }
  sendTimer.cancel();
}

std::uint8_t SendTunnelingState::getSequence() const { return sequence; }

SendTunnelingState::State SendTunnelingState::getState() const {
  return state;
}

void SendTunnelingState::cancel() {
  state = State::canceled;
  sendTimer.cancel();
}

asio::awaitable<void> SendTunnelingState::send(const unsigned int attempts) {
  state = State::sending;
  co_await sendMethod(data);
  sendTimer.expires_after(50ms);
  auto [error] = co_await sendTimer.async_wait(asio::as_tuple);
  if (!error) {
    if (attempts > 1) {
      co_await send(attempts - 1);
    } else {
      state = State::timeout;
    }
  } else if(error != asio::error::operation_aborted) {
      state = State::error;
  } 
}

} // namespace knx::connection
