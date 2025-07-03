
#pragma once

#include "KnxStructure.h"
#include <array>
#include <cstdint>

class ConnectionRequestData : public KnxStructure {
public:
  std::array<std::uint8_t, 8> bodyAsByteArray();

protected:
  void parseBody(ByteBufferReader &byteBuffer, std::uint16_t length) override;

private:
};
