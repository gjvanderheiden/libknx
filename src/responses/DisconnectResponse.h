#pragma once

#include "bytes/ByteBufferReader.h"

namespace knx::requestresponse {

class DisconnectResponse {
public:
  static DisconnectResponse parse(ByteBufferReader& buffer);

  static constexpr std::uint16_t SERVICE_ID = 0x020A;

private:
  std::uint8_t channelId;
  std::uint8_t status;

};

}
