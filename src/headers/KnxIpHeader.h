#pragma once

#include "bytes/ByteBufferReader.h"
#include <cstdint>

class KnxIpHeader {
  static KnxIpHeader createAndParse(ByteBufferReader& bytebuffer);
  std::uint16_t getServiceType();
  std::uint16_t getLengthInBytes();
private:
  std::uint16_t serviceType;
};
