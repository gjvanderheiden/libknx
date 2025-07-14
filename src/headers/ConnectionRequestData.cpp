
#include "ConnectionRequestData.h"

void ConnectionRequestData::parseBody(ByteBufferReader& byteBuffer, std::uint16_t length) {
  byteBuffer.skip(length);
}

void ConnectionRequestData::appendToByteArray(ByteBufferWriter& data) {
  data.writeUint8(2);
  data.writeUint8(getType());
}

