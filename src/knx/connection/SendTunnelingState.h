#pragma once

#include "knx/cemi/Cemi.h"
#include "knx/headers/ConnectionHeader.h"
#include <asio/awaitable.hpp>
#include <asio/io_context.hpp>
#include <asio/steady_timer.hpp>
#include <cstdint>

namespace knx::connection {

/**
 *
 */
class SendTunnelingState {
public:

  enum class State{
    initial, ok, sending, timeout, canceled, error
  };
  

  using SendMethod = std::function<asio::awaitable<void>(ByteSpan)>;

public:
  SendTunnelingState(Cemi && cemi, std::uint8_t channelId, std::uint8_t sequence, asio::io_context &ctx, SendMethod sendMethod);

  [[nodiscard]] std::uint8_t getSequence() const;
  [[nodiscard]] State getState() const;
  void onReceiveAckTunnelResponse(const ConnectionHeader& connectionHeader);
  asio::awaitable<void> send(unsigned int attempts = 2);
  void cancel();

private:
  State state{State::initial};
  std::uint8_t channelId{0};
  std::uint8_t sequence{0};
  asio::steady_timer sendTimer;
  SendMethod sendMethod;
  std::vector<uint8_t> data;
};

} // namespace connection
