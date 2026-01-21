#include "knx/cemi/TCPI.h"

TCPI::TCPI(const bool isControlType, const bool hasSequence, const std::uint8_t sequence)
    : isControl{isControlType}, hasSequence{hasSequence}, sequence(sequence) {
}

bool TCPI::isControlType() const {
  return this->isControl;
}

TCPI TCPI::parse(const byte byte) {
  bool isControl = byte & 1 << 8;
  bool hasSequence = byte & 1 << 7;
  std::uint8_t sequence = byte >> 4 & 0x03;
  return {isControl, hasSequence, sequence};
}

byte TCPI::toByte() const {
  byte asByte{0};
  if(isControl) {
    asByte |= 1 << 8;
  }
  if(hasSequence) {
    asByte |= 1 << 7;
  }
  asByte |= (sequence  & 0x03) << 4;
  return asByte;
}
