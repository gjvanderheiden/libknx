#include "knx/ip/UdpSocket.h"

#include <asio.hpp>
#include <asio/as_tuple.hpp>
#include <asio/error.hpp>
#include <asio/experimental/awaitable_operators.hpp>
#include <asio/ip/address.hpp>
#include <asio/ip/address_v4.hpp>
#include <asio/ip/multicast.hpp>
#include <asio/placeholders.hpp>
#include <asio/socket_base.hpp>
#include <asio/use_awaitable.hpp>
#include <asio/write.hpp>
#include <iostream>
#include <utility>

namespace udp {

using asio::awaitable;
using asio::co_spawn;

constexpr auto use_nothrow_awaitable = asio::as_tuple(asio::use_awaitable);

UdpSocket::UdpSocket(asio::io_context &ctx, std::string_view bindHost,
                     const unsigned short port)
    : ctx{ctx}, bindHost{bindHost}, port{port},
      endpoint{asio::ip::make_address_v4(bindHost), port}, socket{ctx} {}

void UdpSocket::setHandler(DataReceivedFunction &&function) {
  this->handlerFunction = std::move(function);
}

void UdpSocket::setConnectionClosedHandler(SocketClosedFunction &&function) {
  this->onSocketClosedFunction = std::move(function);
}

void UdpSocket::startMulticast(const asio::ip::address &multicastAddress) {
  socket.open(endpoint.protocol());
  socket.set_option(asio::ip::udp::socket::reuse_address(true));
  socket.bind(endpoint);
  socket.set_option(asio::ip::multicast::join_group(multicastAddress));
  co_spawn(ctx, readIncoming(), asio::detached);
  open = true;
}

void UdpSocket::startMulticast(const std::string_view multicastAddress) {
  startMulticast(asio::ip::make_address(multicastAddress));
}

void UdpSocket::start() {
  if (!open) {
    socket.open(endpoint.protocol());
    socket.set_option(asio::ip::udp::socket::reuse_address(true));
    socket.bind(endpoint);
    co_spawn(ctx, readIncoming(), asio::detached);
    open = true;
  }
}

/**
 * Using a lambda instead of co_await on reading the socket.
 * Same implementation detail as readIncomming().
 */
void UdpSocket::receiveSome() {
  socket.async_receive_from(
      asio::buffer(buffer), remoteEndpoint,
      [this](const std::error_code &error, const std::size_t size) {
        if (error == asio::error::operation_aborted) {
          return;
        }
        if (!error) {
          if (size > 0 && this->handlerFunction) {
            std::vector<std::uint8_t> data;
            data.reserve(size);
            std::copy_n(buffer.begin(), size, std::back_inserter(data));
            handlerFunction(std::move(data));
          }
          receiveSome();
        } else {
          this->stop(false);
        }
      });
}

/**
 * Implementation detail: asio also supports a callback method. But when you
 * call socket.close() or socket.cancel() there is still a callback call by asio
 * _AFTER_ the cancel() is called, with error code
 * asio::error::operation_aborted (comes later in the eventloop of asio). This
 * is the nature of co routines.
 *
 * This is a problem when destructing this object, because the object is
 * destroyed and that is obiously UB and not preferred. The best I can do is do
 * a get out of the function on a operation_aborted and hope for the best
 *
 */
awaitable<void> UdpSocket::readIncoming() {
  bool reading = true;
  std::cout << "Start reading socket " << port << "\n";
  while (reading) {
    auto [error, size] = co_await socket.async_receive_from(
        asio::buffer(buffer), remoteEndpoint, use_nothrow_awaitable);
    if (error == asio::error::operation_aborted) {
      std::cerr << "Abort on socket read\n";
      co_return;
    } else if (error) {
      std::cout << "Error reading socket " << port << " : " << error.message()
                << '\n';
      reading = false;
    } else if (size > 0 && this->handlerFunction) {
      std::vector<std::uint8_t> data;
      data.reserve(size);
      std::copy_n(buffer.begin(), size, std::back_inserter(data));
      handlerFunction(std::move(data));
    } else if (size == 0) {
      std::cerr << "Received 0 bytes packet on socket " << port << '\n';
    }
  }
  open = false;
}

void UdpSocket::stop(bool resetHandler) {
  if (open) {
    open = false;
    if (resetHandler) {
      this->handlerFunction = nullptr;
    }
    socket.release();
    if (this->onSocketClosedFunction) {
      this->onSocketClosedFunction();
    }
  }
}

void UdpSocket::writeToSync(const asio::ip::udp::endpoint &address,
                            ByteSpan data) {
  socket.send_to(asio::buffer(data), address);
}

awaitable<void> UdpSocket::writeTo(const asio::ip::udp::endpoint &address,
                                   ByteSpan data) {
  co_await socket.async_send_to(asio::buffer(data), address,
                                asio::use_awaitable);
}

} // namespace udp
