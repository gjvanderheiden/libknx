#pragma once

#include "bytes/ByteBufferReader.h"
#include <cstdint>
#include <vector>

class IpAddress {
public:
  static IpAddress parse(ByteBufferReader& byteBuffer);
  void addToByteArray(std::vector<std::uint8_t>& data);
  std::array<uint8_t,4> address{};
  std::string asString();
};

