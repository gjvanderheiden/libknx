
#include "IpKnxConnection.h"
#include <asio/io_context.hpp>

namespace connection {
IpKnxConnection::IpKnxConnection(asio::io_context &ctx,
                                 std::string_view remoteIp,
                                 std::uint16_t remotePort,
                                 std::uint16_t localDataPort,
                                 std::uint16_t localControlPort)
    : ctx{ctx},
      remoteControlEndPoint{asio::ip::make_address_v4(remoteIp), remotePort},
      dataSocket{ctx, "0.0.0.0", localDataPort},
      controlSocket{ctx, "0.0.0.0", localControlPort} {}
} // namespace connection
