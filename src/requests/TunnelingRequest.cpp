#include "TunnelingRequest.h"
 

TunnelRequest::TunnelRequest(
                                   ConnectionHeader &&connectionHeader, Cemi &&cemi)
    : cemi{std::move(cemi)}, connectionHeader{std::move(connectionHeader)} {}



std::vector<std::uint8_t> TunnelRequest::toBytes(){return{};}

TunnelRequest TunnelRequest::parse(ByteBufferReader& reader) {
  ConnectionHeader connectionHeader;
  connectionHeader.parse(reader);
  return {std::move(connectionHeader), std::move(Cemi::parse(reader))};
}

const ConnectionHeader& TunnelRequest::getConnectionHeader() const{
  return connectionHeader;
}

const Cemi& TunnelRequest::getCemi() const {
  return cemi;
}
