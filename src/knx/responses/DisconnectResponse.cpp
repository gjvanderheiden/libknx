#include "knx/responses/DisconnectResponse.h"
#include "knx/bytes/ByteBufferWriter.h"
#include "knx/headers/KnxIpHeader.h"

namespace knx::requestresponse {

DisconnectResponse::DisconnectResponse(const std::uint8_t channel,
                                       const std::uint8_t status) : channel{channel}, status{status}{}

DisconnectResponse DisconnectResponse::parse(ByteBufferReader &reader) {
  return DisconnectResponse{reader.readUint8(), reader.readUint8()};
}

std::vector<std::uint8_t> DisconnectResponse::toBytes() const {
  std::vector<byte> bytes;
  bytes.reserve(LENGTH);
  ByteBufferWriter writer(bytes);
  KnxIpHeader::write(writer, SERVICE_ID, LENGTH);
  writer.writeUint8(channel);
  writer.writeUint8(status);
  return bytes;
}

} // namespace knx::requestresponse
