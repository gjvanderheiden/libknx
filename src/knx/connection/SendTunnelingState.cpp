#include "knx/connection/SendTunnelingState.h"
#include "knx/requests/TunnelingRequest.h"
#include <asio/as_tuple.hpp>
#include <asio/awaitable.hpp>
#include <asio/detached.hpp>
#include <asio/io_context.hpp>
#include <asio/ip/address_v4.hpp>
#include <asio/steady_timer.hpp>
#include <asio/use_awaitable.hpp>
#include <cstdint>
#include <iostream>

namespace knx::connection {

using namespace std::chrono_literals;

SendTunnelingState::SendTunnelingState(Cemi &&cemi, std::uint8_t channelId,
                                       std::uint8_t sequence,
                                       asio::io_context &ctx,
                                       SendMethod sendMethod)
    : data{TunnelRequest{{channelId, sequence, 0x00}, std::move(cemi)}
               .toBytes()},
      sequence{sequence}, sendTimer{ctx}, sendMethod{sendMethod} {}

void SendTunnelingState::onReceiveAckTunnelResponse(
    const ConnectionHeader &connectionHeader) {
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
  if (error == asio::error::operation_aborted) {
  } else if (error) {
    if(state != State::canceled) {
      state = State::ok;
    }
    std::cout << "got an error for sequence " << +sequence << "closing"
              << std::endl;
  } else {
    std::cout << "time out on sequence " << +sequence << std::endl;
    if (attempts > 1) {
      co_await send(attempts - 1);
    } else {
      state = State::timeout;
    }
  }
}

} // namespace knx::connection
