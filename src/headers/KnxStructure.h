#pragma once

#include "bytes/ByteBufferReader.h"
#include <asio/awaitable.hpp>
#include <cstdint>

class  KnxStructure {
public:
  KnxStructure(){};
  KnxStructure(std::uint16_t type);
  void parse(ByteBufferReader& byteBuffer);
  std::uint8_t getType() const;
protected:
  void setType(std::uint8_t type);
  virtual void parseBody(ByteBufferReader& byteBuffer, std::uint16_t length) = 0;
private:
  std::uint16_t type;
};

