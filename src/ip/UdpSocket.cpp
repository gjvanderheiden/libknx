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
using namespace asio::experimental::awaitable_operators;
using std::chrono::steady_clock;
using namespace std::literals::chrono_literals;

constexpr auto use_nothrow_awaitable = asio::as_tuple(asio::use_awaitable);

awaitable<void> watchdog(steady_clock::time_point &deadline);

UdpSocket::UdpSocket(asio::io_context &ctx, std::string_view bindHost,
                     const unsigned short port)
    : ctx(ctx), bindHost(bindHost), port(port),
      endpoint(asio::ip::make_address_v4(bindHost), port), socket{ctx} {}

void UdpSocket::setHandler(HandlerFunction &&function) {
  this->handlerFunction = std::move(function);
}

void UdpSocket::receiveSome(const std::error_code &error,
                            const std::size_t size) {
  if (this->handlerFunction && size > 0 && !error) {
    std::vector<std::uint8_t> data;
    std::copy_n(buffer.begin(), size, std::back_inserter(data));
    handlerFunction(data);
  }
}

void UdpSocket::startMulticast(std::string_view multicastAddress) {
  socket.open(endpoint.protocol());
  socket.set_option(asio::ip::udp::socket::reuse_address(true));
  socket.bind(endpoint);
  socket.set_option(asio::ip::multicast::join_group(
      asio::ip::make_address(multicastAddress)));
  socket.async_receive_from(asio::buffer(buffer), remoteEndpoint,
                            std::bind_front(&UdpSocket::receiveSome, this));
  open = true;
}
void UdpSocket::start() {
  socket.open(endpoint.protocol());
  socket.bind(endpoint);
  co_spawn(ctx, readIncoming(), asio::detached);
  open = true;
}

awaitable<void> UdpSocket::readIncoming() {
  bool keepRunning = true;
  while (keepRunning) {

    auto [error, size] = co_await socket.async_receive_from(
        asio::buffer(buffer), remoteEndpoint, use_nothrow_awaitable);
    if (error) {
      keepRunning = false;
    } else {
      this->receiveSome(error, size);
    }
  }
}

void UdpSocket::stop() {
  try {
    open = false;
    this->handlerFunction = nullptr;
    socket.cancel();
    socket.close();
  } catch (std::exception &e) {
    std::cout << "Error closing socket: " << e.what() << "\n";
  }
}

void UdpSocket::writeToSync(asio::ip::udp::endpoint address, ByteSpan data) {
  socket.send_to(asio::buffer(data), address);
}

awaitable<void> UdpSocket::writeTo(const asio::ip::udp::endpoint &address,
                                   ByteSpan data) {
  co_await socket.async_send_to(asio::buffer(data), address,
                                asio::use_awaitable);
}

} // namespace udp
