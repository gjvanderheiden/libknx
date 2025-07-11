#pragma once

#include "KnxStructure.h"
#include "bytes/ByteBufferReader.h"

class ConnectionHeader final : public KnxStructure {
public:
  ConnectionHeader();
  
  void appendToByteArray(ByteBufferWriter& data) override;
protected:
  void parseBody(ByteBufferReader& byteBuffer, std::uint16_t length) override;

private:
  std::uint8_t sequence{0};
  std::uint8_t status{0};
};

