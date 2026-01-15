#include "knx/requests/TunnelingRequest.h"
#include "knx/headers/KnxIpHeader.h"

TunnelRequest::TunnelRequest(ConnectionHeader &&connectionHeader, Cemi &&cemi)
    : connectionHeader{std::move(connectionHeader)}, cemi{std::move(cemi)} {}

void TunnelRequest::write(ByteBufferWriter &writer) {
  std::vector<std::uint8_t> requestBytes;
  ByteBufferWriter requestWriter{requestBytes};
  connectionHeader.write(requestWriter);
  cemi.write(requestWriter);

  KnxIpHeader ipHeader{SERVICE_ID,
                       static_cast<std::uint16_t>(requestBytes.size() +
                                                  KnxIpHeader::SIZE_HEADER)};
  ipHeader.write(writer);
  writer.writeVector(requestBytes);
}

TunnelRequest TunnelRequest::parse(ByteBufferReader &reader) {
  return TunnelRequest{ConnectionHeader::parse(reader), Cemi::parse(reader)};
}

const ConnectionHeader &TunnelRequest::getConnectionHeader() const {
  return connectionHeader;
}

const Cemi &TunnelRequest::getCemi() const { return cemi; }
