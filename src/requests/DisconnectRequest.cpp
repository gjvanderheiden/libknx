#include "DisconnectRequest.h"

DisconnectRequest::DisconnectRequest(std::uint8_t channel, HPAI&& hpai) : channel{channel}, controlEndpoint{std::move(hpai)} {
}

DisconnectRequest DisconnectRequest::parse(ByteBufferReader& reader) {
  std::uint8_t channel = reader.readUint8();
  reader.skip(1);
  return {channel, HPAI::createAndParse(reader)};
}
std::uint8_t DisconnectRequest::getChannel() const {
  return channel;
}

const HPAI& DisconnectRequest::getControlEndpoint() const {
  return controlEndpoint;
}
