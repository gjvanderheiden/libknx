#pragma once

#include <asio.hpp>
#include <cstdint>
#include <functional>
#include <vector>
#include <string>
#include <bytes/ByteDef.h>

namespace udp {

using asio::awaitable;
using asio::ip::tcp;
using HandlerFunction = std::function<auto (std::vector<std::uint8_t>& data) -> void>;
using std::chrono::steady_clock;

class UdpSocket {
public:
  UdpSocket(asio::io_context &ctx, std::string bindHost, unsigned short port);
  void setHandler(HandlerFunction function);
  void start(asio::io_context &ctx);
  awaitable<void> write(ByteSpan data);

private:
  std::string bindHost;
  unsigned short port;
  asio::ip::udp::endpoint endpoint;
  asio::ip::udp::socket socket;
  std::unique_ptr<HandlerFunction> handlerFunction;
  void receiveSome(const std::error_code& error, std::size_t size);

  awaitable<void> listen(tcp::acceptor &acceptor);
  awaitable<void> transfer(tcp::socket &socket,
                           steady_clock::time_point &deadline);
  awaitable<void> transferWithTimeOut(tcp::socket socket);
};

} // namespace http_server
