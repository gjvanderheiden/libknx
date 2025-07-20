
#pragma once

#include "KnxStructure.h"
#include <cstdint>

class ConnectionRequestData final : public KnxStructure {
public:
  static constexpr std::uint8_t TUNNELING_TYPE = 4;
  ConnectionRequestData(std::uint8_t type);
  static ConnectionRequestData createAndParse(ByteBufferReader &data);
  static ConnectionRequestData newTunneling();

  void appendToByteArray(ByteBufferWriter &data) const override;

};
