#include "KnxStructure.h"

KnxStructure::KnxStructure(std::uint8_t type) : type{type} {
}

KnxStructure::pr KnxStructure::parse(ByteBufferReader& byteBuffer) {
  std::uint16_t length = byteBuffer.readUint8();
  if (length == MAX_SIZE) {
    length = byteBuffer.readUint16();
  }
  length -= 2;
  return {.length=length, .type=byteBuffer.readUint8()};
}

std::uint8_t KnxStructure::getType() const {
  return type;
}

void KnxStructure::writeKnxStructure(ByteBufferWriter& data, std::uint16_t length) const {
  if (length >= MAX_SIZE) {
    data.writeUint8(MAX_SIZE);
    data.writeUint16(length);
  } else {
    data.writeUint8(length);
  }
  data.writeUint8(type);
}

