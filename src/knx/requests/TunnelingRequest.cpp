#include "knx/requests/TunnelingRequest.h"
#include "knx/headers/KnxIpHeader.h"

TunnelRequest::TunnelRequest(ConnectionHeader &&connectionHeader, Cemi &&cemi)
    : connectionHeader{std::move(connectionHeader)}, cemi{std::move(cemi)} {}

std::vector<std::uint8_t> TunnelRequest::toBytes() {
  std::vector<std::uint8_t> requestBytes;
  ByteBufferWriter requestWriter{requestBytes};
  connectionHeader.write(requestWriter);
  cemi.write(requestWriter);

  std::vector<std::uint8_t> bytes;
  ByteBufferWriter writer{bytes};
  KnxIpHeader ipHeader{SERVICE_ID,
                       static_cast<std::uint16_t>(requestBytes.size() + 6)};
  ipHeader.write(writer);
  writer.writeVector(requestBytes);

  return bytes;
}

TunnelRequest TunnelRequest::parse(ByteBufferReader &reader) {
  return TunnelRequest{ConnectionHeader::parse(reader), Cemi::parse(reader)};
}

const ConnectionHeader &TunnelRequest::getConnectionHeader() const {
  return connectionHeader;
}

const Cemi &TunnelRequest::getCemi() const { return cemi; }
