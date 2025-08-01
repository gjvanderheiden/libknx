#include "knx/responses/DisconnectResponse.h"
#include "knx/bytes/ByteBufferWriter.h"

namespace knx::requestresponse {

DisconnectResponse::DisconnectResponse(const std::uint8_t channel,
                                       const std::uint8_t status) : channel{channel}, status{status}{}

DisconnectResponse DisconnectResponse::parse(ByteBufferReader &reader) {
  return DisconnectResponse{reader.readUint8(), reader.readUint8()};
}

std::vector<std::uint8_t> DisconnectResponse::toBytes() const {
  std::vector<byte> bytes;
  ByteBufferWriter writer(bytes);
  writer.writeUint8(0x06);
  writer.writeUint8(0x10);
  writer.writeUint16(SERVICE_ID);
  writer.writeUint8(0x00);
  writer.writeUint8(8);
  writer.writeUint8(channel);
  writer.writeUint8(status);
  return bytes;
}

} // namespace knx::requestresponse
