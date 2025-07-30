#pragma once

#include "knx/bytes/ByteBufferReader.h"
#include "knx/bytes/ByteBufferWriter.h"
#include <cstdint>

class KnxIpHeader {
public:
  KnxIpHeader(std::uint16_t serviceType, std::uint16_t lengthInBytes);

  void write(ByteBufferWriter& writer);

  static KnxIpHeader parse(ByteBufferReader& reader);

  std::uint16_t getServiceType();
  std::uint16_t getLengthInBytes();
private:
  std::uint16_t serviceType;
  std::uint16_t lengthInBytes;
};
