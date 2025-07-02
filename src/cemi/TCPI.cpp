#include "TCPI.h"

TCPI::TCPI(bool isControl, bool hasSequence, std::uint8_t sequence)
    : isControl{isControl}, hasSequence{hasSequence}, sequence(sequence) {
}


TCPI TCPI::parseAndCreate(ByteBuffer &byteBuffer) {
  std::uint8_t byte = byteBuffer.readUint8();
  bool isControl = byte & (1 << 8);
  bool hasSequence = byte & (1 << 7);
  std::uint8_t sequence = byte & 0x03;
  return {isControl, hasSequence, sequence};
}
