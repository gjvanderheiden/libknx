#pragma once

#include "knx/bytes/ByteBufferReader.h"
#include "knx/bytes/ByteBufferWriter.h"
#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>
#include <vector>

namespace knx::datapoint {

class BooleanDataPointType {
public:
  static constexpr std::array<std::uint8_t, 2> off{0x00, 0x00};

  static constexpr std::array<std::uint8_t, 2> on{0x00, 0x01};

  static std::array<std::uint8_t, 2> toData(bool value) {
    return value ? on : off;
  }

  static bool toValue(std::array<std::uint8_t, 2> data) {
    return data[1] != 0x00;
  }
};

class StringDataPointType {
public:
  static auto toData(std::string_view value, const std::size_t size)
      -> std::vector<std::uint8_t> {
    std::vector<std::uint8_t> data;
    ByteBufferWriter writer(data);
    writer.writeTerminatedString(value, size);
    return data;
  }

  static std::string toValue(std::span<std::uint8_t> data) {
    ByteBufferReader reader(data);
    return reader.readTerminatedString(data.size());
  }
};

} // namespace knx::datapoint
