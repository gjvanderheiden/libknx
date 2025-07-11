#pragma once

#include "bytes/ByteBufferReader.h"
#include <cstdint>

class KnxIpHeader {
public:
  KnxIpHeader() = delete;
  static KnxIpHeader createAndParse(ByteBufferReader& bytebuffer);
  std::uint16_t getServiceType();
  std::uint16_t getLengthInBytes();
private:
  KnxIpHeader(std::uint16_t serviceType, std::uint16_t lengthInBytes);
  std::uint16_t serviceType;
  std::uint16_t lengthInBytes;
};
