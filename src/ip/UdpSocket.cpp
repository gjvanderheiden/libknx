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

void UdpSocket::startMulticast(std::string_view multicastAddress) {
  socket.open(endpoint.protocol());
  socket.set_option(asio::ip::udp::socket::reuse_address(true));
  socket.bind(endpoint);
  socket.set_option(asio::ip::multicast::join_group(
      asio::ip::make_address(multicastAddress)));
  co_spawn(ctx, readIncoming(), asio::detached);
  open = true;
}

void UdpSocket::start() {
  socket.open(endpoint.protocol());
  socket.bind(endpoint);
  co_spawn(ctx, readIncoming(), asio::detached);
  open = true;
}

/**
 * Implementation detail: asio also supports a callback method. But when you
 * call socket.close() or socket.cancel() there is still a callback call by asio
 * _AFTER_ the cancel() is called, with an error code (comes later in the
 * eventloop of asio). Probably by design. This is a problem when destructing
 * this object, because the object is destroyed and that is obiously UB and not
 * preferred.
 *
 * Running a awaitable loop solves this.
 */
awaitable<void> UdpSocket::readIncoming() {
  bool keepRunning = true;
  while (keepRunning) {
    auto [error, size] = co_await socket.async_receive_from(
        asio::buffer(buffer), remoteEndpoint, use_nothrow_awaitable);
    if (error) {
      keepRunning = false;
    } else if(size > 0 && this->handlerFunction) { 
      std::vector<std::uint8_t> data;
      data.reserve(size);
      std::copy_n(buffer.begin(), size, std::back_inserter(data));
      handlerFunction(data);
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
