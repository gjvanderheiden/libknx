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

class BooleanFormat {
public:
  static constexpr bool FITS_IN_6BITS{true};

public:
  static constexpr std::array<const std::uint8_t, 1> OFF{0x00};
  static constexpr std::array<const std::uint8_t, 1> ON{0x01};

  static std::array<const std::uint8_t, 1> toData(bool value) {
    return value ? ON : OFF;
  }

  static bool toValue(std::span<const std::uint8_t> data) {
    return data.size() == 1 && data[0] != 0x00;
  }
};

class StringFormat {
  static constexpr bool FITS_IN_6BITS{false};

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

class UInt8Format {
  static constexpr bool FITS_IN_6BITS{false};

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

class UInt16Format {
  static constexpr bool FITS_IN_6BITS{false};

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

class KnxFloat16Format {

public:
  static auto toData(double number) -> std::array<std::uint8_t, 2> {
    std::array<std::uint8_t, 2> data{0x00};
    // do range check
    if (number <= -671088.64) {
      return data;
    }

    if (number >= 670760.96) {
      return data;
    }

    number = (number * 100);

    std::uint8_t exponent = 0;
    double mantisse = number;

    while ((mantisse < -2048) || (mantisse > 2047)) {
      mantisse /= 2.0;
      exponent++;
    }
    std::vector<std::uint8_t> mantisseBytes;
    mantisseBytes.reserve(2);
    const ByteBufferWriter writer(mantisseBytes);
    writer.writeInt16(static_cast<std::int16_t>(mantisse));
    data[0] = exponent << 3 | (mantisseBytes[0] &0x87);
    data[1] = mantisseBytes[1];
    return data;
  }

  static double toValue(std::span<const std::uint8_t> data) {

    if (data.size() == 2) {
      std::array<std::uint8_t,2> mData;
      mData[0] = data[0] & 0x87;
      mData[1] = data[1];
      if((data[0] & 0x80) == 0x80) {
        mData[0] |= 0x78;
      }
      const double mantisse = static_cast<double>(ByteBufferReader(mData).readInt16());
      const std::uint8_t exp = (data[0] >> 3) & 0x0F;
      return 0.01 * mantisse * pow(2.0, exp);
    } else {
      return 0;
    }
  }
};

class UInt6Format {
public:
  static const bool FITS_IN_6BITS{true};

public:
  static auto toData(std::uint8_t value) -> std::array<std::uint8_t, 1> {
    return {static_cast<std::uint8_t>(value & 0x3F)};
  }

  static std::uint8_t toValue(std::span<const std::uint8_t> data) {
    if (data.size() == 1) {
      return data[0] & 0x3F;
    } else {
      return 0;
    }
  }
};
} // namespace knx::datapoint
