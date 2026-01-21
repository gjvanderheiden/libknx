#pragma once

#include <asio/io_context.hpp>
#include <asio/steady_timer.hpp>
#include <string>
#include <vector>
#include <asio.hpp>
#include <knx/ip/UdpSocket.h>
#include <chrono>
#include <functional>

namespace knx::connection {

struct KnxIp {
  std::string name;
  std::string ip;
  std::uint16_t port;
};
using namespace std::chrono_literals;

using DiscoveryCallback = std::function<void(KnxIp& found)>;

class Discovery {
public:
  explicit Discovery(std::chrono::duration<long> timeOut = 2s);
  std::vector<KnxIp> lookAround(int maxResults = 1);
  void lookAround(DiscoveryCallback&& callback);


private:
  static void doReceive(ByteSpan data, DiscoveryCallback& callback);
  asio::awaitable<void> startScanning(DiscoveryCallback&& callback);

private:
  asio::io_context ctx;
  asio::steady_timer timer{ctx};
  udp::UdpSocket socket;
  std::chrono::duration<long> timeOut;
  asio::ip::udp::endpoint senderEndpoint;
  std::array<std::uint8_t, 100> data{0};
  asio::ip::address multicastAddress;
};

}
