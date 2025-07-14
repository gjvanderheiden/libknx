
#pragma once

#include "KnxStructure.h"
#include <cstdint>

class ConnectionRequestData final : public KnxStructure {
public:
  static constexpr std::uint8_t TUNNELING_TYPE = 4;
  ConnectionRequestData() : KnxStructure(TUNNELING_TYPE){};
  void appendToByteArray(ByteBufferWriter &data) override;

protected:
  void parseBody(ByteBufferReader &byteBuffer, std::uint16_t length) override;
};
