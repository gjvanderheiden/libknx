#include "knx/responses/TunnelAckResponse.h"

namespace knx::requestresponse {

TunnelAckResponse::TunnelAckResponse(ConnectionHeader&& connectionHeader): connectionHeader{std::move(connectionHeader)} {}

TunnelAckResponse TunnelAckResponse::parse(ByteBufferReader& reader) {
  return {ConnectionHeader::parse(reader)};
}

std::vector<byte> TunnelAckResponse::toBytes() const {
  std::vector<byte> bytes;
  ByteBufferWriter writer(bytes);
  writer.writeUint8(0x06);
  writer.writeUint8(0x10);
  writer.writeUint16(SERVICE_ID);
  writer.writeUint8(0x00);
  writer.writeUint8(10);
  connectionHeader.write(writer);
  return bytes;
}


}
