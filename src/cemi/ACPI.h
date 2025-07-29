#pragma once

#include "ByteBufferWriter.h"
#include "ByteBufferReader.h"
#include <array>
#include <cstdint>


class DataACPI {
public:
  DataACPI(std::uint8_t type, std::array<std::uint8_t, 2> data);
  DataACPI(std::uint8_t type);
  // 10 bit
  static constexpr std::uint8_t GROUP_VALUE_READ = 0b0000;
  static constexpr std::uint8_t INDIVIDUAL_ADDRESS_READ = 0b0100;
  static constexpr std::uint8_t INDIVIDUAL_ADDRESS_WRITE = 0b0101;
  // 4 bit
  static constexpr std::uint8_t GROUP_VALUE_RESPONSE = 0b0001;
  static constexpr std::uint8_t GROUP_VALUE_WRITE = 0b0010;

  static DataACPI parse(byte firstByte, byte length, ByteBufferReader &reader);
  void write(byte firstByte, ByteBufferWriter &writer) const;

  [[nodiscard]] std::uint8_t getType() const;
  [[nodiscard]] std::array<byte,2> getData() const;

private:
  std::uint8_t type{0};
  std::array<byte, 2> data{0};
};

enum class ControlType {
  Connect, Disconnect, Nack, Ack
};


class ControlACPI {
public:
  explicit ControlACPI(ControlType controlType);
  static ControlACPI parse(byte firstByte, byte length, ByteBufferReader &reader);
  [[nodiscard]] ControlType getControlType() const;
private:
  ControlType controlType;
};
