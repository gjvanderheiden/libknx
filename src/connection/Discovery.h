#pragma once

#include <string>
#include <vector>
#include <asio.hpp>

namespace connection {

struct KnxIp {
  std::string name;
  std::string ip;
};

class Discovery {
public:
  explicit Discovery(asio::io_context& io_context);
  // no time out implemented
  void lookAround(int maxResults = 1);
  std::vector<KnxIp>& result();
private:
  void do_receive();
  asio::ip::udp::socket socket_read;
  asio::ip::udp::endpoint sender_endpoint;
  std::array<std::uint8_t, 1024> data;
  asio::ip::address multicast_address;
  std::vector<KnxIp> foundKnxIps{};
  int maxResults{0};
};

}
