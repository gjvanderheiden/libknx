#pragma once
#include "knx/headers/KnxStructure.h"
#include "knx/bytes/ByteBufferWriter.h"

class ConnectionRequestInformation final : public KnxStructure {
public:
  static constexpr std::uint8_t TUNNELING_CONNECTION_TYPE = 0x04;
  static constexpr std::uint8_t DEVICE_MANAGEMENT_CONNECTION_TYPE = 0x03;
  static constexpr std::uint8_t REMOTE_LOGGING_CONNECTION_TYPE = 0x06;
  static constexpr std::uint8_t REMOTE_CONF_CONNECTION_TYPE = 0x07;
  static constexpr std::uint8_t OBJECT_SERVER_CONNECTION_TYPE = 0x08;

  static constexpr std::uint8_t TUNNELING_LINK_LAYER = 0x02;
  
public:
  ConnectionRequestInformation(std::uint8_t connectionType, std::uint8_t layer);
  static ConnectionRequestInformation newTunneling();

  static ConnectionRequestInformation parse(ByteBufferReader& reader);
  void write(ByteBufferWriter &writer) const override;

private:
  const std::uint8_t layer{TUNNELING_LINK_LAYER};
};
