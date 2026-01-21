#pragma once

#include "knx/connection/SendTunnelingState.h"
#include <asio/as_tuple.hpp>
#include <asio/awaitable.hpp>
#include <asio/detached.hpp>
#include <asio/error.hpp>
#include <asio/io_context.hpp>
#include <asio/steady_timer.hpp>

namespace knx::connection {


using namespace std::chrono_literals;

template <typename DataType, typename ResponseType>
SendMessageState<DataType, ResponseType>::SendMessageState(asio::io_context &ctx,
                                       SendMethod &&sendMethod, MatchMethod&& matchMethod)

    : sendTimer{ctx}, sendMethod{std::move(sendMethod)}, matchMethod{std::move(matchMethod)} {}


template <typename DataType,typename ResponseType >
bool SendMessageState<DataType, ResponseType>::matchResponse(ResponseType&& response) {
  bool match = matchMethod(response);
  if(match) {
    state = State::ok;
    sendTimer.cancel();
    this->response = std::move(response);
  }
  return match;
}

template <typename DataType, typename ResponseType>
ResponseType SendMessageState<DataType, ResponseType>::getResponse() {
  return response;
}

template <typename DataType, typename ResponseType>
SendMessageState<DataType, ResponseType>::State SendMessageState<DataType, ResponseType>::getState() const { return state; }

template <typename DataType, typename ResponseType>
void SendMessageState<DataType, ResponseType>::cancel() {
  state = State::canceled;
  sendTimer.cancel();
}


template <typename DataType, typename ResponseType>
asio::awaitable<void> SendMessageState<DataType, ResponseType>::send(const unsigned int attempts) {
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
