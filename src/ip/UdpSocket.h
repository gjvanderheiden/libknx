#pragma once

#include <asio.hpp>
#include <bytes/ByteDef.h>
#include <cstdint>
#include <functional>
#include <string>
#include <vector>

namespace udp {

using asio::awaitable;
using asio::ip::tcp;
using HandlerFunction =
    std::function<auto(std::vector<std::uint8_t>&& data)->void>;
using std::chrono::steady_clock;

class UdpSocket {
public:
  UdpSocket(asio::io_context &ctx, std::string_view bindHost,
            unsigned short port);
  void setHandler(HandlerFunction &&function);
  void start();
  void stop();
  void startMulticast(asio::ip::address multicastAddress);
  void startMulticast(std::string_view multicastAddress);
  void writeToSync(asio::ip::udp::endpoint address, ByteSpan data);
  awaitable<void> writeTo(const asio::ip::udp::endpoint &address,
                          ByteSpan data);

private:
  awaitable<void> readIncoming();

private:
  asio::io_context &ctx;
  std::array<std::uint8_t, 512> buffer{0};
  std::string bindHost;
  unsigned short port;
  bool open{false};
  asio::ip::udp::endpoint endpoint;
  asio::ip::udp::socket socket;
  asio::ip::udp::endpoint remoteEndpoint;
  HandlerFunction handlerFunction{nullptr};

};

} // namespace udp
