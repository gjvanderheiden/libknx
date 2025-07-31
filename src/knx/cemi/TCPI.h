#pragma once

#include "knx/bytes/ByteDef.h"
#include <cstdint>

class TCPI {
public:
  TCPI(bool isControl, bool hasSequence, std::uint8_t sequence);

  static TCPI parse(byte byte);
  [[nodiscard]] byte toByte() const;

  [[nodiscard]] bool isControl() const;

private:
  bool _isControl{false};
  bool hasSequence{false};
  std::uint8_t sequence{0};
};
