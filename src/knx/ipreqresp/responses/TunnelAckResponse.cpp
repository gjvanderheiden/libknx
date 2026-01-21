#include "knx/ipreqresp/responses/TunnelAckResponse.h"
#include "knx/ipreqresp/KnxIpHeader.h"

namespace knx::requestresponse {

TunnelAckResponse::TunnelAckResponse(ConnectionHeader&& connectionHeader): connectionHeader{std::move(connectionHeader)} {}

TunnelAckResponse TunnelAckResponse::parse(ByteBufferReader& reader) {
  return TunnelAckResponse{ConnectionHeader::parse(reader)};
}

void TunnelAckResponse::write(ByteBufferWriter &writer) const {
  connectionHeader.write(writer);
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
