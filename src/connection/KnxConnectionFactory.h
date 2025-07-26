#pragma once

#include <KnxClientConnection.h>

namespace connection {

class KnxConnectionFactory {
public:
  static KnxClientConnection createTunnelingClient(asio::io_context &ctx, std::string_view remoteIp,
                  std::uint16_t remotePort, std::string_view localBindIp,
                  std::uint16_t localDataPort = 2001,
                  std::uint16_t localControlPort = 2002);
};

} // namespace connection
