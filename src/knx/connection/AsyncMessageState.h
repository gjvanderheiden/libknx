#pragma once

#include <asio/awaitable.hpp>
#include <asio/io_context.hpp>
#include <asio/steady_timer.hpp>

namespace knx::connection {

/**
 * I track exactly 1 send request. I need a send method and a match method to delegate to.
 *
 * Implementation of sending and matching must be provided to me. I'm capable of
 * asynchronic message tracking:
 *
 * - sending a message 
 * - wait for the response
 * - resend if no matching response is received within the time out
 * - track which state the acknowledge is in
 * - cancel the time out (e.g. when closing connection)
 *
 * I take care of the mechanism of resending in short.
 */
template <typename DataType, typename ResponseType>
class AsyncMessageState {
public:
  enum class State : std::uint8_t {
    initial,
    ok,
    sending,
    timeout,
    canceled,
    error,
  };

  using SendMethod = std::function<asio::awaitable<void>()>;
  using MatchMethod = std::function<bool (const ResponseType& response)>;

public:

  AsyncMessageState(asio::io_context &ctx, SendMethod &&sendMethod, MatchMethod&& matchMethod);

  [[nodiscard]] State getState() const;

  asio::awaitable<void> send(unsigned int attempts = 2);

  /**
   * I will stop waiting for an ackResponse() and stop
   * resending. State will be canceled after this
   */
  void cancel();

  /**
   * I will check if the response corresponds with the provided matcher.
   * If the repsonse matches I will return true and the State will be ok.
   * If the response doesn't match, no change and I will return false
   */
  bool matchResponse(const ResponseType& response) const;

  void setResponse(ResponseType&& response);

  ResponseType getResponse();
  

private:
  State state{State::initial};
  asio::steady_timer sendTimer;
  SendMethod sendMethod;
  MatchMethod matchMethod;
  ResponseType response;
};


} // namespace knx::connection
  
