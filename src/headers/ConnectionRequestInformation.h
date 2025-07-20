#pragma once

#include "KnxStructure.h"
#include "bytes/ByteBufferWriter.h"
#include <cstdint>

class ConnectionRequestInformation final : public KnxStructure {
public:
  static constexpr std::uint8_t TUNNELING_CONNECTION_TYPE = 0x04;
  static constexpr std::uint8_t TUNNELING_LINK_LAYER = 0x02;
  
  ConnectionRequestInformation(std::uint8_t connectionType, std::uint8_t layer);
  static ConnectionRequestInformation newTunneling();

  static ConnectionRequestInformation createAndParse(ByteBufferReader& reader);
  void appendToByteArray(ByteBufferWriter &data) const;

private:
  std::uint8_t layer{TUNNELING_LINK_LAYER};
};
