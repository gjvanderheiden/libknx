#include "knx/responses/TunnelAckResponse.h"
#include "knx/headers/KnxIpHeader.h"

namespace knx::requestresponse {

TunnelAckResponse::TunnelAckResponse(ConnectionHeader&& connectionHeader): connectionHeader{std::move(connectionHeader)} {}

TunnelAckResponse TunnelAckResponse::parse(ByteBufferReader& reader) {
  return TunnelAckResponse{ConnectionHeader::parse(reader)};
}

std::vector<byte> TunnelAckResponse::toBytes() const {
  std::vector<byte> bytes;
  bytes.reserve(LENGTH);
  ByteBufferWriter writer(bytes);
  KnxIpHeader::write(writer, SERVICE_ID, LENGTH);
  connectionHeader.write(writer);
  return bytes;
}

const ConnectionHeader& TunnelAckResponse::getConnectionHeader() const {
  return connectionHeader;
}

}
