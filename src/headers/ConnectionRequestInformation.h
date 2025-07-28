#pragma once

#include "KnxStructure.h"
#include "ByteBufferWriter.h"
#include <cstdint>

class ConnectionRequestInformation final : public KnxStructure {
public:
  static constexpr std::uint8_t TUNNELING_CONNECTION_TYPE = 0x04;
  static constexpr std::uint8_t TUNNELING_LINK_LAYER = 0x02;
  
public:
  ConnectionRequestInformation(std::uint8_t connectionType, std::uint8_t layer);
  static ConnectionRequestInformation newTunneling();

  static ConnectionRequestInformation parse(ByteBufferReader& reader);
  void write(ByteBufferWriter &writer) const override;

private:
  std::uint8_t layer{TUNNELING_LINK_LAYER};
};
