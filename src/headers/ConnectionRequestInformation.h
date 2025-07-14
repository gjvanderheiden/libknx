#pragma once

#include "KnxStructure.h"
#include "bytes/ByteBufferWriter.h"
#include <cstdint>

class ConnectionRequestInformation final : public KnxStructure {
public:
  static constexpr std::uint8_t TUNNELING_CONNECTION_TYPE = 0x04;
  static constexpr std::uint8_t TUNNELING_LINK_LAYER = 0x02;
  ConnectionRequestInformation(std::uint8_t connectionType, std::uint8_t layer);
  static ConnectionRequestInformation defaultTunnelingCRI();

  void appendToByteArray(ByteBufferWriter &data) override;

protected:
  void parseBody(ByteBufferReader &byteBuffer, std::uint16_t length) override;

private:
  std::uint8_t layer{TUNNELING_LINK_LAYER};
};
