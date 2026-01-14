#pragma once

#include <asio/io_context.hpp>
#include <asio/steady_timer.hpp>
#include <string>
#include <vector>
#include <asio.hpp>
#include <knx/ip/UdpSocket.h>
#include <chrono>
namespace knx::connection {

struct KnxIp {
  std::string name;
  std::string ip;
  std::uint16_t port;
};
using namespace std::chrono_literals;


class Discovery {
public:
  explicit Discovery(asio::io_context& ctx, std::chrono::duration<long> timeOut = 2s);
  // no time out implemented
  void lookAround(int maxResults = 1);
  std::vector<KnxIp>& result();


private:
  void doReceive(std::vector<std::uint8_t>&& data);
  asio::awaitable<void> runTimeOut();

private:
  asio::io_context& ctx;
  udp::UdpSocket socket;
  std::chrono::duration<long> timeOut;
  asio::ip::udp::endpoint senderEndpoint;
  std::array<std::uint8_t, 1024> data{};
  asio::ip::address multicastAddress;
  std::vector<KnxIp> foundKnxIps{};
  int maxResults{0};
  asio::steady_timer timer;
};

}
