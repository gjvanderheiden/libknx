#pragma once

#include "knx/bytes/ByteDef.h"
#include <cstdint>

class TCPI {
public:
  TCPI(bool isControlType, bool hasSequence, std::uint8_t sequence);

  static TCPI parse(byte byte);
  [[nodiscard]] byte toByte() const;
  [[nodiscard]] bool isControlType() const;

private:
  bool isControl;
  bool hasSequence;
  std::uint8_t sequence;
};
