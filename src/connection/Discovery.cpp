#include "Discovery.h"
#include "HPAI.h"
#include "IpAddress.h"
#include "KnxIpHeader.h"
#include "SearchRequest.h"
#include "bytes/ByteBufferReader.h"
#include "responses/SearchResponse.h"
#include <asio.hpp>
#include <asio/io_context.hpp>
#include <asio/ip/address_v4.hpp>
#include <cstddef>
#include <iostream>

namespace connection {

constexpr short multicast_port = 3671;

Discovery::Discovery(asio::io_context &io_context)
    : socket_read(io_context),
      multicast_address(asio::ip::make_address_v4("224.0.23.12")) {
  asio::ip::address listen_address = asio::ip::make_address_v4("0.0.0.0");

  asio::ip::udp::endpoint listen_endpoint(listen_address, multicast_port);
  socket_read.open(listen_endpoint.protocol());
  socket_read.set_option(asio::ip::udp::socket::reuse_address(true));
  socket_read.bind(listen_endpoint);

  socket_read.set_option(asio::ip::multicast::join_group(multicast_address));
}

void Discovery::lookAround(int maxResults) {
  this->maxResults = maxResults;
  IpAddress ipAddress;
  ipAddress.address = {224, 0, 23, 12};
  HPAI local{ipAddress, 3671, HPAI::UDP};
  SearchRequest searchRequest{local};
  std::vector<byte> srBytes;
  ByteBufferWriter writer(srBytes);
  searchRequest.appendToByteWriter(writer);
  do_receive();

  asio::ip::udp::endpoint endpoint{multicast_address, multicast_port};
  socket_read.async_send_to(asio::buffer(srBytes), endpoint,
                            [](std::error_code ec, std::size_t length) {});
}

void Discovery::do_receive() {
  socket_read.async_receive_from(
      asio::buffer(data), sender_endpoint,
      [this](std::error_code ec, std::size_t length) {
        if (!ec) {
        ByteBufferReader reader{data};
          KnxIpHeader knxIpHeader = KnxIpHeader::createAndParse(reader);
          if (knxIpHeader.getServiceType() == knx::requestresponse::SearchResponse::SERVICE_ID) {
            ByteBufferReader bytebuffer{this->data};
            knx::requestresponse::SearchResponse sr =
                knx::requestresponse::SearchResponse::parse(reader);

            foundKnxIps.emplace_back(
                std::string{sr.getDeviceDib().getDeviceName()},
                sr.getControlEndPoint().getAddress().asString(),
                sr.getControlEndPoint().getPort());
            if (maxResults == 0 || foundKnxIps.size() < maxResults) {
              do_receive();
            } else {
               socket_read.close();
            }

          } else {
            do_receive();
          }
        } else {
          std::cout << "Error code =  " << ec << "\n";
        }
      });
}

std::vector<KnxIp> &Discovery::result() { return foundKnxIps; }
} // namespace connection
