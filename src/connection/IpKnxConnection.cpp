
#include "IpKnxConnection.h"
#include "responses/ResponseFactory.h"
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
      controlSocket{ctx, "0.0.0.0", localControlPort} {
  dataSocket.setHandler(std::bind_front(&IpKnxConnection::onReceiveData, this));
  controlSocket.setHandler(std::bind_front(&IpKnxConnection::onReceiveData, this));

}

void IpKnxConnection::onReceiveData(std::vector<std::uint8_t>& data) {
  ByteBufferReader reader(data);
  knx::requestresponse::ResponseFactory::parse(reader);
}

void IpKnxConnection::setGroupData(GroupAddress &ga, bool value) {
  // on receive packet: check if seq / response type is
  // awaited for 
  //   if yes : send packet to interested boeliewoelie.
  //
  // obtain next sequence
  // construct DataPacket
  // construct TunnelingPacket
  // register for send
  // send
  //
  // if event receieved: hurray, check if status is ok.
  
}

bool IpKnxConnection::getGroupData(GroupAddress &ga) { 
  // on receive packet: check if seq/response type is awaited for
  //   if yes : send packet to interseted boeliewoelie
  //
  //   obtain next sequence
  //   construct DataPacket
  //   construct TunnelingPacket
  //   register for send
  //   send

  return false; }

void IpKnxConnection::close() {}

} // namespace connection
