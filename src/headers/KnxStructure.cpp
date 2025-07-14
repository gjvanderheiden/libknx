#include "KnxStructure.h"

KnxStructure::KnxStructure(std::uint16_t type) : type{type} {
}
void KnxStructure::parse(ByteBufferReader& byteBuffer) {
  std::uint16_t length = byteBuffer.readUint8();
  if (length == 0xFF) {
    length = byteBuffer.readUint16();
  }
  this->type = byteBuffer.readUint8();
  this->parseBody(byteBuffer, length - 2);
}

std::uint8_t KnxStructure::getType() const {
  return type;
}


void KnxStructure::setType(std::uint8_t type) {
  this->type = type;
}
