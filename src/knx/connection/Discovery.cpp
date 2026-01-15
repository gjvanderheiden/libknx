#include "knx/connection/Discovery.h"
#include "knx/bytes/ByteBufferReader.h"
#include "knx/headers/HPAI.h"
#include "knx/headers/IpAddress.h"
#include "knx/headers/KnxIpHeader.h"
#include "knx/requests/SearchRequest.h"
#include "knx/responses/SearchResponse.h"
#include <asio.hpp>
#include <asio/io_context.hpp>
#include <asio/ip/address_v4.hpp>
#include <chrono>
#include <functional>

namespace knx::connection {

using namespace std::chrono_literals;
constexpr short MULTICAST_PORT = 3671;
constexpr std::array<std::uint8_t, 4> MULTICAST_ADDRESS{244, 0, 23, 12};

Discovery::Discovery(asio::io_context &ctx,
                     const std::chrono::duration<long> timeOut)
    : ctx{ctx}, socket(ctx, "0.0.0.0", MULTICAST_PORT), timeOut{timeOut},
      multicastAddress(asio::ip::make_address_v4(MULTICAST_ADDRESS)),
      timer{ctx} {}

namespace {
std::vector<uint8_t> makeSearchRequest() {
  IpAddress ipAddress{MULTICAST_ADDRESS[0], MULTICAST_ADDRESS[1],
                      MULTICAST_ADDRESS[2], MULTICAST_ADDRESS[3]};
  HPAI local{ipAddress, MULTICAST_PORT, HPAI::UDP};
  SearchRequest searchRequest{std::move(local)};
  std::vector<byte> srBytes;
  ByteBufferWriter writer(srBytes);
  searchRequest.write(writer);
  return srBytes;
}
} // namespace
  
void Discovery::lookAround(int maxResults) {
  this->maxResults = maxResults;

  socket.setHandler(std::bind_front(&Discovery::doReceive, this));
  socket.startMulticast(multicastAddress);

  asio::ip::udp::endpoint endpoint{multicastAddress, MULTICAST_PORT};
  auto searchRequest = makeSearchRequest();
  socket.writeToSync(endpoint, searchRequest);
  co_spawn(ctx, runTimeOut(), asio::detached);
}

asio::awaitable<void> Discovery::runTimeOut() {
  timer.expires_after(timeOut);
  co_await timer.async_wait();
  socket.stop();
}

void Discovery::doReceive(std::vector<std::uint8_t> &&data) {
  ByteBufferReader reader{std::move(data)};
  if (const KnxIpHeader knxIpHeader = KnxIpHeader::parse(reader); knxIpHeader.getServiceType() ==
                                                                  knx::requestresponse::SearchResponse::SERVICE_ID) {
    const knx::requestresponse::SearchResponse sr =
        knx::requestresponse::SearchResponse::parse(reader);
    foundKnxIps.emplace_back(std::string{sr.getDeviceDib().getDeviceName()},
                             sr.getControlEndPoint().getAddress().asString(),
                             sr.getControlEndPoint().getPort());
    if (foundKnxIps.size() == maxResults) {
      timer.cancel();
      socket.stop();
    }
  }
}

std::vector<KnxIp> &Discovery::result() { return foundKnxIps; }
} // namespace knx::connection
