#pragma once

#include "bytes/ByteBuffer.h"
#include <cstdint>

class KnxIpHeader {
  static KnxIpHeader parse(ByteBuffer& bytebuffer);
  std::uint16_t getServiceType();
  std::uint16_t getLengthInBytes();
private:
  std::uint16_t serviceType;
};
