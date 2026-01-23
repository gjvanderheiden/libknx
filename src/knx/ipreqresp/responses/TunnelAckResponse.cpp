#include "knx/ipreqresp/responses/TunnelAckResponse.h"
#include "knx/ipreqresp/KnxIpHeader.h"
#include <sstream>

namespace knx::requestresponse {
using std::stringstream;

TunnelAckResponse::TunnelAckResponse(ConnectionHeader &&connectionHeader)
    : connectionHeader{std::move(connectionHeader)} {}

TunnelAckResponse TunnelAckResponse::parse(ByteBufferReader &reader) {
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

const ConnectionHeader &TunnelAckResponse::getConnectionHeader() const {
  return connectionHeader;
}

std::string TunnelAckResponse::getShortDescription() const {
  stringstream stringstream;
  stringstream << "TunnelAckResponse [channel: "
               << static_cast<int>(getConnectionHeader().getChannel())
               << ", sequence : "
               << static_cast<int>(getConnectionHeader().getSequence()) << "]";
  return stringstream.str();
}
} // namespace knx::requestresponse
