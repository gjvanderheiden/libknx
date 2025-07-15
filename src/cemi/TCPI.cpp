#include "TCPI.h"

TCPI::TCPI(bool isControl, bool hasSequence, std::uint8_t sequence)
    : _isControl{isControl}, hasSequence{hasSequence}, sequence(sequence) {
}

bool TCPI::isControl() const {
  return this->_isControl;
}

TCPI TCPI::parseAndCreate(byte byte) {
  bool isControl = byte & (1 << 8);
  bool hasSequence = byte & (1 << 7);
  std::uint8_t sequence = byte & 0x03;
  return {isControl, hasSequence, sequence};
}
