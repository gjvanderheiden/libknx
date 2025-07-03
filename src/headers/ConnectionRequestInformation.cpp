#include "ConnectionRequestInformation.h"

void ConnectionRequestInformation::parseBody(ByteBufferReader& byteBuffer, std::uint16_t length) {
  byteBuffer.skip(length);
}


void ConnectionRequestInformation::appendToByteArray(std::vector<std::uint8_t>& data) {
  data.push_back(0x00);
}
