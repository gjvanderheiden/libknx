#pragma once

#include <cstdint>

class TCPI {
private:
  bool isControl;
  bool hasSequence;
  std::uint8_t sequence;
};
