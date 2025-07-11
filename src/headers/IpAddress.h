#pragma once

#include "bytes/ByteBufferReader.h"
#include "bytes/ByteBufferWriter.h"
#include <cstdint>

class IpAddress {
public:
  static IpAddress parse(ByteBufferReader& byteBuffer);
  void appendToByteArray(ByteBufferWriter& data);
  std::array<uint8_t,4> address{};
  std::string asString();
};

