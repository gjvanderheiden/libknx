#pragma once

#include "ConnectionHeader.h"
#include "bytes/ByteBuffer.h"

namespace knx::requestresponse {

class TunnelAckResponse {
public:
  static TunnelAckResponse parse(ByteBuffer& buffer);

  static constexpr std::array<std::uint8_t, 2> SERVICE_ID = {0x02, 0x21};

private:
  ConnectionHeader connectionHeader;

};

}
