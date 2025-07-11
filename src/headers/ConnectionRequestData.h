
#pragma once

#include "KnxStructure.h"
#include <array>
#include <cstdint>

class ConnectionRequestData final : public KnxStructure {
public:
  std::array<std::uint8_t, 8> bodyAsByteArray();
  void appendToByteArray(ByteBufferWriter& data) override;

protected:
  void parseBody(ByteBufferReader &byteBuffer, std::uint16_t length) override;

private:
};
