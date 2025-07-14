#pragma once

#include "ConnectionHeader.h"
#include "bytes/ByteBufferReader.h"

namespace knx::requestresponse {

class TunnelAckResponse {
public:
  TunnelAckResponse(ConnectionHeader&& connectionHeader);
  static TunnelAckResponse parse(ByteBufferReader& buffer);
  std::vector<byte> toBytes();

  static constexpr std::uint16_t SERVICE_ID = 0x0421;

private:
  ConnectionHeader connectionHeader;

};

}
