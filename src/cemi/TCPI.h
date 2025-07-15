#pragma once

#include "bytes/ByteBufferReader.h"
#include <cstdint>

class TCPI {
public:
  TCPI(bool isControl, bool hasSequence, std::uint8_t sequence);
  static TCPI parseAndCreate(byte byte);
  bool isControl() const;
private:
  bool _isControl{false};
  bool hasSequence{false};
  std::uint8_t sequence{0};
};
