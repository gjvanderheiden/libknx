#pragma once

#include "bytes/ByteBuffer.h"

namespace knx::requestresponse {

class ConnectStateResponse {
public:
  static ConnectStateResponse parse(ByteBuffer& buffer);

  static constexpr std::array<std::uint8_t, 2> SERVICE_ID = {0x02, 0x08};

private:
  std::uint8_t channelId;
  std::uint8_t status;

};

}
