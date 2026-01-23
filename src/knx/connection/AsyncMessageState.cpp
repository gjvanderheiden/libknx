#include "knx/connection/AsyncMessageState.h"
#include "knx/cemi/Cemi.h"
#include "knx/ipreqresp/requests/AbstractRequest.h"
#include <asio/as_tuple.hpp>
#include <asio/awaitable.hpp>
#include <asio/detached.hpp>
#include <asio/error.hpp>
#include <asio/io_context.hpp>
#include <asio/steady_timer.hpp>

namespace knx::connection {

using namespace std::chrono_literals;

template <typename DataType, typename ResponseType>
AsyncMessageState<DataType, ResponseType>::AsyncMessageState(
    asio::io_context &ctx, SendMethod &&sendMethod, MatchMethod &&matchMethod)

    : sendTimer{ctx}, sendMethod{std::move(sendMethod)},
      matchMethod{std::move(matchMethod)} {}

template <typename DataType, typename ResponseType>
bool AsyncMessageState<DataType, ResponseType>::matchResponse(
    const ResponseType &response) const {
  return matchMethod(response);
}

template <typename DataType, typename ResponseType>
void AsyncMessageState<DataType, ResponseType>::setResponse(
    ResponseType &&response) {
  state = State::ok;
  sendTimer.cancel();
  this->response = std::move(response);
}

template <typename DataType, typename ResponseType>
ResponseType AsyncMessageState<DataType, ResponseType>::getResponse() {
  return response;
}

template <typename DataType, typename ResponseType>
AsyncMessageState<DataType, ResponseType>::State
AsyncMessageState<DataType, ResponseType>::getState() const {
  return state;
}

template <typename DataType, typename ResponseType>
void AsyncMessageState<DataType, ResponseType>::cancel() {
  std::cout << "Cancelled\n";
  state = State::canceled;
  sendTimer.cancel();
}

template <typename DataType, typename ResponseType>
asio::awaitable<void>
AsyncMessageState<DataType, ResponseType>::send(unsigned int attempts) {
  state = State::sending;
  while (state == State::sending) {
    attempts--;
    co_await sendMethod();
    if (state == State::ok || state == State::canceled) {
      break;
    }
    sendTimer.expires_after(200ms);
    auto [error] = co_await sendTimer.async_wait(asio::as_tuple);
    if (!error && attempts == 0) {
      std::cout << "Timeout\n";
      state = State::timeout;
    } else if (error && error != asio::error::operation_aborted) {
      state = State::error;
    }
  }
}

template class AsyncMessageState<Cemi, std::optional<Cemi>>;
template class AsyncMessageState<AbstractRequest,
                                 requestresponse::ResponseVariant>;
} // namespace knx::connection
