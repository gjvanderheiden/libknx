#pragma once

#include "bytes/ByteBuffer.h"
#include <cstdint>

class IpAddress {
public:
  static IpAddress parse(ByteBuffer& byteBuffer);
  std::array<uint8_t,4> address{};
  std::string asString();
};

