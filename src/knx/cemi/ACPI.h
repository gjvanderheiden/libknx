#pragma once

#include "knx/bytes/ByteBufferWriter.h"
#include "knx/bytes/ByteBufferReader.h"
#include <cstdint>
#include <vector>
#include <span>


class DataACPI {
public:
  DataACPI(std::uint8_t type, std::vector<byte>&& data, bool fits6Bits = false);
  DataACPI(std::uint8_t type, std::span<const byte> data);
  DataACPI(std::uint8_t type, byte byteData, bool fits6Bits);
  DataACPI(std::uint8_t type);

  static constexpr std::uint8_t GROUP_VALUE_READ = 0b0000;
  static constexpr std::uint8_t INDIVIDUAL_ADDRESS_READ = 0b0100;
  static constexpr std::uint8_t INDIVIDUAL_ADDRESS_WRITE = 0b0101;
  // these can compact data of 6 bits into second byte in the networkframe:
  static constexpr std::uint8_t GROUP_VALUE_RESPONSE = 0b0001;
  static constexpr std::uint8_t GROUP_VALUE_WRITE = 0b0010;

  static DataACPI parse(byte firstByte, byte length, ByteBufferReader &reader);
  void write(byte firstByte, ByteBufferWriter &writer) const;

  [[nodiscard]] std::uint8_t getType() const;
  [[nodiscard]] std::vector<byte> getData() const;

private:
  bool fits6Bits{false};
  std::uint8_t type{0};
  std::vector<byte> data{};
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
