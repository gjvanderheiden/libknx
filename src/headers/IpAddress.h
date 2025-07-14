#pragma once

#include "bytes/ByteBufferReader.h"
#include "bytes/ByteBufferWriter.h"
#include <cstdint>

class IpAddress {
public:
  IpAddress() = default;
  IpAddress(std::array<uint8_t,4>&& asArray);
  static IpAddress parse(ByteBufferReader& byteBuffer);
  void appendToByteArray(ByteBufferWriter& data);
  std::array<uint8_t,4> address{0};
  std::string asString();
};

