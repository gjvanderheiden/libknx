#include "UdpSocket.h"

#include <asio.hpp>
#include <asio/as_tuple.hpp>
#include <asio/experimental/awaitable_operators.hpp>
#include <asio/placeholders.hpp>
#include <chrono>
#include <iostream>

namespace http_server {

using asio::awaitable;
using asio::buffer;
using asio::co_spawn;
using asio::detached;
using asio::ip::udp;
namespace this_coro = asio::this_coro;
using namespace asio::experimental::awaitable_operators;
using std::chrono::steady_clock;
using namespace std::literals::chrono_literals;

constexpr auto use_nothrow_awaitable = asio::as_tuple(asio::use_awaitable);

awaitable<void> watchdog(steady_clock::time_point &deadline);
UdpSocket::UdpSocket(std::string bindHost, unsigned short port)
    : bindHost(std::move(bindHost)), port(port) {}

void UdpSocket::setHandler(HandlerFunction function) {
  this->handlerFunction.reset(&function);
}

void UdpSocket::receiveSome(const std::error_code &error, std::size_t size) {}

void UdpSocket::start() {
  try {
    asio::io_context ctx;
    udp::socket socket(ctx, udp::endpoint{udp::v4(), port});
    std::array<std::uint8_t, 512> buffer;
    udp::endpoint remoteEndpoint;
    socket.async_receive_from(asio::buffer(buffer), remoteEndpoint,
                              std::bind_front(&UdpSocket::receiveSome, this));

    ctx.run();
  } catch (std::exception &e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }
}

awaitable<void> UdpSocket::listen(tcp::acceptor &acceptor) {
  for (;;) {
    auto [e, client] = co_await acceptor.async_accept(use_nothrow_awaitable);
    if (e)
      break;

    co_spawn(acceptor.get_executor(),
             this->transferWithTimeOut(std::move(client)), detached);
  }
}

awaitable<void> UdpSocket::transferWithTimeOut(tcp::socket socket) {
  steady_clock::time_point deadline{};
  co_await (this->transfer(socket, deadline) || watchdog(deadline));
}

awaitable<void> UdpSocket::transfer(tcp::socket &socket,
                                    steady_clock::time_point &deadline) {
  // std::array<char, 1024> data;
  char data[1024];
  for (;;) {
    deadline = std::max(deadline, steady_clock::now() + 6s);

    auto [e1, n1] =
        co_await socket.async_read_some(buffer(data), use_nothrow_awaitable);
    if (e1)
      co_return;

    // (*(this->handlerFunction.get()))(buffer);

    // auto [e2, n2] =
    //   co_await async_write(socket, buffer(to_string(response), n1),
    //   use_nothrow_awaitable);
    // if (e2)
  }
}

awaitable<void> watchdog(steady_clock::time_point &deadline) {
  asio::steady_timer timer(co_await this_coro::executor);

  auto now = steady_clock::now();
  while (deadline > now) {
    timer.expires_at(deadline);
    co_await timer.async_wait(use_nothrow_awaitable);
    now = steady_clock::now();
  }
}

} // namespace http_server
