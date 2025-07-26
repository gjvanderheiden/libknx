#pragma once

#include <asio/io_context.hpp>
#include <cstdint>
#include <string>
#include <vector>
#include <asio.hpp>
#include <ip/UdpSocket.h>

namespace connection {

struct KnxIp {
  std::string name;
  std::string ip;
  std::uint16_t port;
};

class Discovery {
public:
  explicit Discovery(asio::io_context& ctx);
  // no time out implemented
  void lookAround(int maxResults = 1);
  std::vector<KnxIp>& result();

private:
  void do_receive(std::vector<std::uint8_t>&& data);

private:
  udp::UdpSocket socket;
  asio::io_context* ctx;
  asio::ip::udp::endpoint senderEndpoint;
  std::array<std::uint8_t, 1024> data;
  asio::ip::address multicastAddress;
  std::vector<KnxIp> foundKnxIps{};
  int maxResults{0};
};

}
