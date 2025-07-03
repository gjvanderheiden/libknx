
#include "ConnectionRequestData.h"

void ConnectionRequestData::parseBody(ByteBufferReader& byteBuffer, std::uint16_t length) {
  byteBuffer.skip(length);
}
