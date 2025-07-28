#pragma once

#include "ByteBufferWriter.h"
#include "ByteBufferReader.h"
#include <asio/awaitable.hpp>

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
  const std::uint8_t type{0};
};
