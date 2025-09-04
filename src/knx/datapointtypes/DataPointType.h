#pragma once

#include "knx/bytes/ByteBufferReader.h"
#include "knx/bytes/ByteBufferWriter.h"
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace knx::datapoint {

class BooleanDataPointType {
public:
  static const bool FITS_IN_6BITS{true};

public:
  static constexpr std::array<const std::uint8_t, 1> off{0x00};

  static constexpr std::array<const std::uint8_t, 1> on{0x01};

  static std::array<const std::uint8_t, 1> toData(bool value) {
    return value ? on : off;
  }

  static bool toValue(std::span<const std::uint8_t> data) {
    return data.size() == 1 & data[0] != 0x00;
  }
};

class StringDataPointType {
  static const bool FITS_IN_6BITS{false};

public:
  static auto toData(std::string_view value, const std::size_t size)
      -> std::vector<std::uint8_t> {
    std::vector<std::uint8_t> data;
    ByteBufferWriter writer(data);
    writer.writeTerminatedString(value, size);
    return data;
  }

  static std::string toValue(std::span<const std::uint8_t> data) {
    ByteBufferReader reader(data);
    return reader.readTerminatedString(data.size());
  }
};

class UInt8DataPointType {
  static const bool FITS_IN_6BITS{false};

public:
  static auto toData(std::uint8_t value) -> std::array<std::uint8_t, 1> {
    return {value};
  }

  static std::uint8_t toValue(std::span<const std::uint8_t> data) {
    if (data.size() == 1) {
      return data[0];
    } else {
      return 0x00;
    }
  }
};

class UInt16DataPointType {
  static const bool FITS_IN_6BITS{false};

public:
  static auto toData(std::uint8_t value) -> std::array<std::uint8_t, 1> {
    return {value};
  }

  static std::uint16_t toValue(std::span<const std::uint8_t> data) {
    if (data.size() == 2) {
      return ByteBufferReader(data).readUint16();
    } else {
      return 0;
    }
  }
};

class KnxFloat16DataPointType {
public:
  /**  static auto toData(double value)
        -> std::array<std::uint8_t, 2> {


    }
  **/
  static double toValue(std::span<const std::uint8_t> data) {
    if (data.size() == 2) {
      std::uint8_t m_data[2];
      m_data[0] = data[0] & static_cast<std::uint8_t>(0b10000111);
      m_data[1] = data[1];
      double m = static_cast<double>(ByteBufferReader(m_data).readInt16());
      std::uint8_t e = data[0] >> 3;
      e = e & 0x0F;
      return 0.01 * m * pow(2.0, e);
    } else {
      return 0;
    }
  }
};

class KnxSceneDataPointType {
public:
  static const bool FITS_IN_6BITS{true};

public:
  static auto toData(std::uint8_t value) -> std::array<std::uint8_t, 1> {
    return {value};
  }

  static std::uint8_t toValue(std::span<const std::uint8_t> data) {
    if (data.size() == 1) {
      return data[0];
    } else {
      return 0;
    }
  }
};
} // namespace knx::datapoint
