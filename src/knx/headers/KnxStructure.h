#pragma once

#include "knx/bytes/ByteBufferWriter.h"
#include "knx/bytes/ByteBufferReader.h"
#include <asio/awaitable.hpp>
#include <cstdint>


enum class KnxMedium : std::uint8_t {
  KNX_TP = 0x01,
  PL110 = 0x04,
  RF = 0x10,
  KNX_IP = 0x20,
};


class KnxStructure {
public:
  virtual ~KnxStructure() = default;
  explicit KnxStructure(std::uint8_t type);
  [[nodiscard]] std::uint8_t getType() const;

  virtual void write(ByteBufferWriter& writer) const = 0;

protected:
  struct pr{std::uint16_t length; std::uint8_t type;};
  static pr parse(ByteBufferReader &byteBuffer);
  void writeKnxStructure(ByteBufferWriter& data, std::uint16_t length) const;

private:
  static constexpr std::uint8_t MAX_SIZE = 255;
  std::uint8_t type{0};
};
