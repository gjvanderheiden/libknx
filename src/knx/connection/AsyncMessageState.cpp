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
AsyncMessageState<DataType, ResponseType>::AsyncMessageState(asio::io_context &ctx,
                                       SendMethod &&sendMethod, MatchMethod&& matchMethod)

    : sendTimer{ctx}, sendMethod{std::move(sendMethod)}, matchMethod{std::move(matchMethod)} {}


template <typename DataType,typename ResponseType >
bool AsyncMessageState<DataType, ResponseType>::matchResponse(ResponseType&& response) {
  bool match = matchMethod(response);
  if(match) {
    state = State::ok;
    sendTimer.cancel();
    this->response = std::move(response);
  }
  return match;
}

template <typename DataType, typename ResponseType>
ResponseType AsyncMessageState<DataType, ResponseType>::getResponse() {
  return response;
}

template <typename DataType, typename ResponseType>
AsyncMessageState<DataType, ResponseType>::State AsyncMessageState<DataType, ResponseType>::getState() const { return state; }

template <typename DataType, typename ResponseType>
void AsyncMessageState<DataType, ResponseType>::cancel() {
  state = State::canceled;
  sendTimer.cancel();
}


template <typename DataType, typename ResponseType>
asio::awaitable<void> AsyncMessageState<DataType, ResponseType>::send(const unsigned int attempts) {
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

template class  AsyncMessageState<Cemi, std::optional<Cemi>>;
template class  AsyncMessageState<AbstractRequest, requestresponse::ResponseVariant>;
} // namespace knx::connection
