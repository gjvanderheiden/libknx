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
#include <functional>
#include <chrono>

namespace knx::connection {

using namespace std::chrono_literals;
constexpr short multicast_port = 3671;

Discovery::Discovery(asio::io_context &ctx, std::chrono::duration<long> timeout)
    : ctx{ctx}, timeOut{timeout}, socket(ctx, "0.0.0.0", multicast_port),
      multicastAddress(asio::ip::make_address_v4("224.0.23.12")),
      timer{ctx}{}

std::vector<uint8_t> makeSearchRequest() {
  IpAddress ipAddress{224, 0, 23, 12};
  HPAI local{ipAddress, 3671, HPAI::UDP};
  SearchRequest searchRequest{std::move(local)};
  std::vector<byte> srBytes;
  ByteBufferWriter writer(srBytes);
  searchRequest.appendToByteWriter(writer);
  return srBytes;
}

void Discovery::lookAround(int maxResults) {
  this->maxResults = maxResults;

  socket.setHandler(std::bind_front(&Discovery::do_receive, this));
  socket.startMulticast(multicastAddress);

  asio::ip::udp::endpoint endpoint{multicastAddress, multicast_port};
  auto searchRequest = makeSearchRequest();
  socket.writeToSync(endpoint, searchRequest);
  co_spawn(ctx, runTimeOut(), asio::detached);
}

asio::awaitable<void> Discovery::runTimeOut() {
  timer.expires_after(timeOut);
  co_await timer.async_wait();
  socket.stop();
}

void Discovery::do_receive(std::vector<std::uint8_t> &&data) {
  ByteBufferReader reader{data};
  KnxIpHeader knxIpHeader = KnxIpHeader::parse(reader);
  if (knxIpHeader.getServiceType() ==
      knx::requestresponse::SearchResponse::SERVICE_ID) {
    ByteBufferReader bytebuffer{this->data};
    knx::requestresponse::SearchResponse sr =
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
