#include "ConnectionRequestInformation.h"
#include "KnxStructure.h"

ConnectionRequestInformation::ConnectionRequestInformation(
    std::uint8_t connectionType, std::uint8_t layer)
    : KnxStructure{connectionType}, layer{layer} {}

ConnectionRequestInformation ConnectionRequestInformation::defaultTunnelingCRI() {
  return {TUNNELING_CONNECTION_TYPE, TUNNELING_LINK_LAYER}; 
}

void ConnectionRequestInformation::parseBody(ByteBufferReader &byteBuffer,
                                             std::uint16_t length) {
  byteBuffer.skip(length);
}

void ConnectionRequestInformation::appendToByteArray(ByteBufferWriter &data) {
  data.writeUint8(4);
  data.writeUint8(getType());
  data.writeUint8(layer);
  data.writeUint8(0x00);
}
