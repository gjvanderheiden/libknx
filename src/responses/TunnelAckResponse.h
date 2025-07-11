#pragma once

#include "ConnectionHeader.h"
#include "bytes/ByteBufferReader.h"

namespace knx::requestresponse {

class TunnelAckResponse {
public:
  static TunnelAckResponse parse(ByteBufferReader& buffer);

  static constexpr std::uint16_t SERVICE_ID = 0x0221;

private:
  ConnectionHeader connectionHeader;

};

}
