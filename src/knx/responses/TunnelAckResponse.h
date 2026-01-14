#pragma once

#include "knx/headers/ConnectionHeader.h"
#include "knx/bytes/ByteBufferReader.h"

namespace knx::requestresponse {

class TunnelAckResponse {
public:
  static constexpr std::uint16_t SERVICE_ID = 0x0421;
  static constexpr std::uint16_t LENGTH = 10;
public:
  explicit TunnelAckResponse(ConnectionHeader&& connectionHeader);

  static TunnelAckResponse parse(ByteBufferReader& reader);
  [[nodiscard]] std::vector<byte> toBytes() const;

  [[nodiscard]] const ConnectionHeader& getConnectionHeader() const;

private:
  ConnectionHeader connectionHeader;
};

}
