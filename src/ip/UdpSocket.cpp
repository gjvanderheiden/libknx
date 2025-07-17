#include "UdpSocket.h"

#include <asio.hpp>
#include <asio/as_tuple.hpp>
#include <asio/experimental/awaitable_operators.hpp>
#include <asio/ip/address.hpp>
#include <asio/ip/address_v4.hpp>
#include <asio/ip/multicast.hpp>
#include <asio/placeholders.hpp>
#include <asio/socket_base.hpp>
#include <asio/use_awaitable.hpp>
#include <asio/write.hpp>
#include <chrono>
#include <iostream>
#include <utility>

namespace udp {

using asio::awaitable;
using asio::co_spawn;
using asio::detached;
namespace this_coro = asio::this_coro;
using namespace asio::experimental::awaitable_operators;
using std::chrono::steady_clock;
using namespace std::literals::chrono_literals;

constexpr auto use_nothrow_awaitable = asio::as_tuple(asio::use_awaitable);

awaitable<void> watchdog(steady_clock::time_point &deadline);

UdpSocket::UdpSocket(asio::io_context &ctx, std::string_view bindHost,
                     const unsigned short port)
    : bindHost(bindHost), port(port), endpoint(asio::ip::make_address_v4(bindHost), port),
      socket{ctx} {}

using HandlerFunction =
    std::function<auto(std::vector<std::uint8_t> &data)->void>;

void UdpSocket::setHandler(HandlerFunction&& function) {
  this->handlerFunction = std::move(function);
}

void UdpSocket::receiveSome(const std::error_code &error, const std::size_t size) {
  if (this->handlerFunction && size > 0 && !error) {
    std::vector<std::uint8_t> data;
    std::copy_n(buffer.begin(), size, std::back_inserter(data));
    handlerFunction(data);
  }
  socket.async_receive_from(asio::buffer(buffer), remoteEndpoint,
                            std::bind_front(&UdpSocket::receiveSome, this));
}

void UdpSocket::startMulticast(std::string_view multicastAddress) {
  socket.open(endpoint.protocol());
  socket.set_option(asio::ip::udp::socket::reuse_address(true));
  socket.bind(endpoint);
  socket.set_option(
      asio::ip::multicast::join_group(asio::ip::make_address(multicastAddress)));
  socket.async_receive_from(asio::buffer(buffer), remoteEndpoint,
                            std::bind_front(&UdpSocket::receiveSome, this));
}
void UdpSocket::start() {
  socket.open(endpoint.protocol());
  socket.bind(endpoint);
  socket.async_receive_from(asio::buffer(buffer), remoteEndpoint,
                            std::bind_front(&UdpSocket::receiveSome, this));
}

void UdpSocket::stop() {
  try {
    socket.close();
  } catch(std::exception& e) {
    std::cout << "Error closing socket: " << e.what() << "\n";
  }
}

void UdpSocket::writeToSync(asio::ip::udp::endpoint address, ByteSpan data) {
  socket.send_to(asio::buffer(data), address);
}

awaitable<void> UdpSocket::writeTo(asio::ip::udp::endpoint address, ByteSpan data) {
  co_await socket.async_send_to(asio::buffer(data), address, asio::use_awaitable);
}

awaitable<void> UdpSocket::listen(tcp::acceptor &acceptor) {
  for (;;) {
    auto [e, client] = co_await acceptor.async_accept(use_nothrow_awaitable);
    if (e) {
      std::cout << "stopped listening" << std::endl;
      break;
    }

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
  std::vector<std::uint8_t> data;
  for (;;) {
    deadline = std::max(deadline, steady_clock::now() + 6s);

    auto [e1, n1] =
        co_await socket.async_read_some(asio::buffer(data), use_nothrow_awaitable);
    if (e1)
      co_return;

    // (*(this->handlerFunction.get()))(buffer);

    // auto [e2, n2] =
    //   co_await async_write(socket, buffer(to_string(response), n1),
    //   use_nothrow_awaitable);
    // if (e2)
    //  
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
