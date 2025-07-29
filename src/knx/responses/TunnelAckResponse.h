#pragma once

#include "knx/headers/ConnectionHeader.h"
#include "knx/bytes/ByteBufferReader.h"

namespace knx::requestresponse {

class TunnelAckResponse {
public:
  static constexpr std::uint16_t SERVICE_ID = 0x0421;
public:
  TunnelAckResponse(ConnectionHeader&& connectionHeader);

  static TunnelAckResponse parse(ByteBufferReader& buffer);
  std::vector<byte> toBytes();

private:
  ConnectionHeader connectionHeader;
};

}
