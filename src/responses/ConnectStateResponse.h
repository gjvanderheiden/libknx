#pragma once

#include "bytes/ByteBufferReader.h"

namespace knx::requestresponse {

class ConnectStateResponse {
public:
  static ConnectStateResponse parse(ByteBufferReader& buffer);
  static constexpr std::uint16_t SERVICE_ID = 0x0208;

private:
  std::uint8_t channelId;
  std::uint8_t status;
};

}
