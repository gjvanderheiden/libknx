#include "DisconnectRequest.h"

DisconnectRequest::DisconnectRequest(std::uint8_t channel, HPAI&& hpai) : channel{channel}, controlEndpoint{std::move(hpai)} {
}

std::vector<std::uint8_t> DisconnectRequest::toBytes() {
  std::vector<std::uint8_t> bytes;
  ByteBufferWriter writer{bytes};
  writer.writeUint8(0x06);
  writer.writeUint8(0x10);
  writer.writeUint16(SERVICE_ID);
  writer.writeUint16(16);
  writer.writeUint8(channel);
  writer.writeUint8(0x00);
  controlEndpoint.write(writer);
  return bytes;
}

DisconnectRequest DisconnectRequest::parse(ByteBufferReader& reader) {
  std::uint8_t channel = reader.readUint8();
  reader.skip(1);
  return {channel, HPAI::parse(reader)};
}
std::uint8_t DisconnectRequest::getChannel() const {
  return channel;
}

const HPAI& DisconnectRequest::getControlEndpoint() const {
  return controlEndpoint;
}
