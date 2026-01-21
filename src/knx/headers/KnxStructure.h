#pragma once

#include "knx/bytes/ByteBufferReader.h"
#include "knx/bytes/ByteBufferWriter.h"
#include <asio/awaitable.hpp>
#include <cstdint>

enum class KnxMedium : std::uint8_t {
  knxTp = 0x01,
  pl110 = 0x04,
  rf = 0x10,
  knxIp = 0x20,
};

class KnxStructure {
public:
  KnxStructure() = default;
  virtual ~KnxStructure() = default;
  KnxStructure(const KnxStructure &) = default;
  KnxStructure(KnxStructure &&) = default;
  KnxStructure &operator=(const KnxStructure &) = default;
  KnxStructure &operator=(KnxStructure &&) = default;
  explicit KnxStructure(std::uint8_t type);

  [[nodiscard]] std::uint8_t getType() const;

  virtual void write(ByteBufferWriter &writer) const = 0;

protected:
  struct ParseResult {
    std::uint16_t length;
    std::uint8_t type;
  };
  static ParseResult parse(ByteBufferReader &byteBuffer);
  void writeKnxStructure(ByteBufferWriter &data, std::uint16_t length) const;

private:
  static constexpr std::uint8_t MAX_SIZE = 255;
  std::uint8_t type{0};
};
