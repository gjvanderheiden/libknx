#include "ConnectionRequestInformation.h"

void ConnectionRequestInformation::parseBody(ByteBufferReader& byteBuffer, std::uint16_t length) {
  byteBuffer.skip(length);
}

void ConnectionRequestInformation::appendToByteArray(ByteBufferWriter& data) {
}

