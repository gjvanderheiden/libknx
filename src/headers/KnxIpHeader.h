#pragma once

#include "ByteBufferReader.h"
#include <cstdint>

class KnxIpHeader {
public:
  KnxIpHeader() = delete;

  static KnxIpHeader parse(ByteBufferReader& reader);

  std::uint16_t getServiceType();
  std::uint16_t getLengthInBytes();
private:
  KnxIpHeader(std::uint16_t serviceType, std::uint16_t lengthInBytes);
  std::uint16_t serviceType;
  std::uint16_t lengthInBytes;
};
