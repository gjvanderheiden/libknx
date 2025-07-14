#pragma once

#include "bytes/ByteBufferReader.h"
#include "bytes/ByteBufferWriter.h"
#include <asio/awaitable.hpp>

class KnxStructure {
public:
  virtual ~KnxStructure() = default;

  KnxStructure() = default;
  explicit KnxStructure(std::uint16_t type);
  void parse(ByteBufferReader &byteBuffer);
  virtual void appendToByteArray(ByteBufferWriter &data) = 0;
  [[nodiscard]] std::uint8_t getType() const;

protected:
  void setType(std::uint8_t type);
  virtual void parseBody(ByteBufferReader &byteBuffer,
                         std::uint16_t length) = 0;

private:
  std::uint16_t type{0};
};
