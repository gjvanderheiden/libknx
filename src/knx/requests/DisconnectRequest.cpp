#include "knx/requests/DisconnectRequest.h"
#include "knx/headers/KnxIpHeader.h"

DisconnectRequest::DisconnectRequest(const std::uint8_t channel, HPAI&& hpai) : channel{channel}, controlEndpoint{std::move(hpai)} {
}

void DisconnectRequest::write(ByteBufferWriter& writer) {
  KnxIpHeader{SERVICE_ID, SIZE}.write(writer);
  writer.writeUint8(channel);
  writer.writeUint8(0x00);
  controlEndpoint.write(writer);
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
