#include "ConnectionRequestInformation.h"

void ConnectionRequestInformation::parseBody(ByteBuffer& byteBuffer, std::uint16_t length) {
  byteBuffer.skip(length);
}
