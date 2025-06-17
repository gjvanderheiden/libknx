#include "UdpSocket.h"
#include <asio/io_context.hpp>
#include <cstdint>
#include <string_view>
namespace connection {

class IpKnxConnection {
public:
  IpKnxConnection(asio::io_context& ctx, std::string_view remoteIp, std::uint16_t remotePort, std::uint16_t localDataPort=2001, std::uint16_t localControlPort=2002);

private:
  void checkConnection();

private:
  asio::io_context& ctx;
  std::uint16_t dataPort;
  asio::ip::udp::endpoint remoteControlEndPoint;
  std::uint16_t controlPort;
  udp::UdpSocket dataSocket;
  udp::UdpSocket controlSocket;
};

}
