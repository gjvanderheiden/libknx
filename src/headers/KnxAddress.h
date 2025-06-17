#pragma once

#include "bytes/ByteBuffer.h"
#include <cstdint>
class IndividualAddress {
public:
  static IndividualAddress createAndParse(ByteBuffer& bytebuffer);
  std::uint8_t getArea();
  std::uint8_t getLine();
  std::uint8_t getDevice();
  
private:
  std::array<std::uint8_t, 2> address;
};

class GroupAddress {
};
