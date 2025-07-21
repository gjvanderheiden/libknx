#include "TunnelingRequest.h"

TunnelRequest::TunnelRequest(ConnectionHeader &&connectionHeader, Cemi &&cemi)
    : cemi{std::move(cemi)}, connectionHeader{std::move(connectionHeader)} {}

std::vector<std::uint8_t> TunnelRequest::toBytes() {
  std::vector<std::uint8_t> bytes;
  ByteBufferWriter writer{bytes};
  writer.writeUint8(0x06);
  writer.writeUint8(0x10);
  writer.writeUint16(SERVICE_ID);
  writer.writeUint16(21);
  connectionHeader.write(writer);
  cemi.toBytes(writer);
  return bytes;
}

TunnelRequest TunnelRequest::parse(ByteBufferReader &reader) {
  return TunnelRequest{ConnectionHeader::parse(reader),
                       Cemi::parse(reader)};
}

const ConnectionHeader &TunnelRequest::getConnectionHeader() const {
  return connectionHeader;
}

const Cemi &TunnelRequest::getCemi() const { return cemi; }
