#pragma once

#include "bytes/ByteBufferReader.h"
#include <cstdint>

class TCPI {
public:
  TCPI() = default;
  TCPI(bool isControl, bool hasSequence, std::uint8_t sequence);
  static TCPI parseAndCreate(ByteBufferReader& ByteBuffer);
private:
  bool isControl{false};
  bool hasSequence{false};
  std::uint8_t sequence{0};
};
