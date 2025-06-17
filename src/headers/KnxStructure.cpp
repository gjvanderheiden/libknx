#include "KnxStructure.h"

void KnxStructure::parse(ByteBuffer& byteBuffer) {
  std::uint16_t length = byteBuffer.readUint8();
  if (length == 0xFF) {
    length = byteBuffer.readUint16();
  }
  this->type = byteBuffer.readUint8();
  this->parseBody(byteBuffer, length);
}

std::uint8_t KnxStructure::getType() const {
  return type;
}


void KnxStructure::setType(std::uint8_t type) {
  this->type = type;
}
