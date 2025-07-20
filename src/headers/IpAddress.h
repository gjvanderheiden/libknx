#pragma once

#include "bytes/ByteBufferReader.h"
#include "bytes/ByteBufferWriter.h"
#include <cstdint>

class IpAddress {
public:
  IpAddress(std::uint8_t byte1, std::uint8_t byte2, std::uint8_t byte3, std::uint8_t byte4);
  IpAddress(std::array<std::uint8_t,4>&& asArray);
  static IpAddress parse(ByteBufferReader& byteBuffer);
  void appendToByteArray(ByteBufferWriter& data) const;
  std::string asString();
private:
  const std::array<std::uint8_t,4> address{0};
};

