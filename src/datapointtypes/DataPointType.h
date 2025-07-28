#pragma once


#include <array>
#include <cstdint>

namespace knx::datapoint {

class BooleanDataPointType {
public:
  static constexpr std::array<std::uint8_t, 2> off {0x00,0x00};

  static constexpr std::array<std::uint8_t, 2> on {0x00,0x00};

  static std::array<std::uint8_t,2> toData(bool value) {
    return value?on:off;
  }

  static bool toValue(std::array<std::uint8_t,2> data) {
    return data[1] != 0x00;
  }
};

} // namespace udp
