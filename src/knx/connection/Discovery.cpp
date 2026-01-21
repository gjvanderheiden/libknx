#include "knx/connection/Discovery.h"
#include "knx/bytes/ByteBufferReader.h"
#include "knx/headers/HPAI.h"
#include "knx/headers/IpAddress.h"
#include "knx/ipreqresp/KnxIpHeader.h"
#include "knx/ipreqresp/requests/SearchRequest.h"
#include "knx/ipreqresp/responses/SearchResponse.h"
#include <asio.hpp>
#include <asio/detached.hpp>
#include <asio/io_context.hpp>
#include <asio/ip/address_v4.hpp>
#include <chrono>
#include <cstdint>
#include <utility>

namespace knx::connection {

using namespace std::chrono_literals;
constexpr short MULTICAST_PORT = 3671;
constexpr std::array<std::uint8_t, 4> MULTICAST_ADDRESS{224, 0, 23, 12};

Discovery::Discovery(const std::chrono::duration<long> timeOut)
    : socket(ctx, "0.0.0.0", MULTICAST_PORT), timeOut{timeOut},
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

std::vector<KnxIp> Discovery::lookAround(int maxResults) {
  std::vector<KnxIp> result;
  lookAround([maxResults, &result, this](KnxIp &knxIp) {
    result.push_back(knxIp);
    if (result.size() >= maxResults) {
      timer.cancel();
    }
  });
  return result;
}

void Discovery::lookAround(DiscoveryCallback &&callback) {
  co_spawn(ctx, startScanning(std::move(callback)), asio::detached);
  ctx.run();
}

asio::awaitable<void> Discovery::startScanning(DiscoveryCallback &&callback) {
  DiscoveryCallback cb = std::move(callback);
  timer.expires_after(timeOut);
  socket.setHandler(
      [&cb](ByteSpan data) { doReceive(data, cb); });
  socket.startMulticast(multicastAddress);

  asio::ip::udp::endpoint endpoint{multicastAddress, MULTICAST_PORT};
  auto searchRequest = makeSearchRequest();
  socket.writeToSync(endpoint, searchRequest);
  co_await timer.async_wait();
  socket.stop();
}

void Discovery::doReceive(ByteSpan data,
                          DiscoveryCallback &callback) {
  ByteBufferReader reader{std::move(data)};
  if (const KnxIpHeader knxIpHeader = KnxIpHeader::parse(reader);
      knxIpHeader.getServiceType() ==
      knx::requestresponse::SearchResponse::SERVICE_ID) {
    const knx::requestresponse::SearchResponse searchResp =
        knx::requestresponse::SearchResponse::parse(reader);
    KnxIp knxIp{.name = std::string{searchResp.getDeviceDib().getDeviceName()},
                .ip = searchResp.getControlEndPoint().getAddress().asString(),
                .port = searchResp.getControlEndPoint().getPort()};
    callback(knxIp);
  }
}

} // namespace knx::connection
