#pragma once

#include "DeviceDib.h"
#include "HPAI.h"
#include "bytes/ByteBuffer.h"

namespace knx::requestresponse {

class DisconnectResponse {
public:
  static DisconnectResponse parse(ByteBuffer& buffer);

  static constexpr std::array<std::uint8_t, 2> SERVICE_ID = {0x02, 0x0A};

private:
  std::uint8_t channelId;
  std::uint8_t status;

};

}
