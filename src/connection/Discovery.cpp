#include "Discovery.h"
#include "HPAI.h"
#include "IpAddress.h"
#include "KnxIpHeader.h"
#include "SearchRequest.h"
#include "ByteBufferReader.h"
#include "SearchResponse.h"
#include <asio.hpp>
#include <asio/io_context.hpp>
#include <asio/ip/address_v4.hpp>
#include <functional>

namespace connection {

constexpr short multicast_port = 3671;

Discovery::Discovery(asio::io_context &ctx)
    : socket(ctx, "0.0.0.0", multicast_port),
      multicastAddress(asio::ip::make_address_v4("224.0.23.12")) {}

std::vector<uint8_t> makeSearchRequest() {
  IpAddress ipAddress{224, 0, 23, 12};
  HPAI local{ipAddress, 3671, HPAI::UDP};
  SearchRequest searchRequest{local};
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
      socket.stop();
    }
  }
}

std::vector<KnxIp> &Discovery::result() { return foundKnxIps; }
} // namespace connection
