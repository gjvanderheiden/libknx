#pragma once

#include <asio.hpp>
#include <cstdint>
#include <functional>
#include <vector>
#include <string>

namespace udp {

using asio::awaitable;
using asio::ip::tcp;
using HandlerFunction = std::function<void(std::vector<std::uint8_t>& data)>;
using std::chrono::steady_clock;

class UdpSocket {
public:
  UdpSocket(std::string bindHost, unsigned short port);
  void setHandler(HandlerFunction function);
  void start();

private:
  std::string bindHost;
  unsigned short port;
  std::unique_ptr<HandlerFunction> handlerFunction;
  void receiveSome(const std::error_code& error, std::size_t size);

  awaitable<void> listen(tcp::acceptor &acceptor);
  awaitable<void> transfer(tcp::socket &socket,
                           steady_clock::time_point &deadline);
  awaitable<void> transferWithTimeOut(tcp::socket socket);
};

} // namespace http_server
