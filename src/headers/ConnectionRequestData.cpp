
#include "ConnectionRequestData.h"

void ConnectionRequestData::parseBody(ByteBuffer& byteBuffer, std::uint16_t length) {
  byteBuffer.skip(length);
}
