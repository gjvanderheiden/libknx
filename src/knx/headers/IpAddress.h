#pragma once

#include "knx/bytes/ByteBufferReader.h"
#include "knx/bytes/ByteBufferWriter.h"
#include <cstdint>

class IpAddress {
public:
  IpAddress() = default;
  IpAddress(const IpAddress &) = default;
  IpAddress(IpAddress &&) = default;
  IpAddress &operator=(const IpAddress &) = default;
  IpAddress &operator=(IpAddress &&) = default;

  IpAddress(std::uint8_t byte1, std::uint8_t byte2, std::uint8_t byte3,
            std::uint8_t byte4);
  explicit IpAddress(std::array<std::uint8_t,4>&& asArray);

  static IpAddress parse(ByteBufferReader& reader);
  void write(ByteBufferWriter& writer) const;

  std::string asString();

private:
  std::array<std::uint8_t,4> address{0};
};

