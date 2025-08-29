#include "KnxConnectionFactory.h"
#include <memory>

namespace knx::connection {

KnxClientConnection KnxConnectionFactory::createTunnelingClient(asio::io_context &ctx, std::string_view remoteIp,
                  std::uint16_t remotePort, std::string_view localBindIp,
                  std::uint16_t localDataPort,
                  std::uint16_t localControlPort) {
  std::unique_ptr tunnelingConnection = std::make_unique<TunnelingConnection>(ctx,remoteIp, remotePort, localBindIp, localDataPort, localControlPort);
  return KnxClientConnection{std::move(tunnelingConnection)};
}

} // namespace connection
