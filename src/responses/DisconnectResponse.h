#pragma once

#include "bytes/ByteBufferReader.h"
#include <cstdint>
#include <vector>

namespace knx::requestresponse {

class DisconnectResponse {
public:
  DisconnectResponse(std::uint8_t channel, std::uint8_t status = 0);
  static DisconnectResponse parse(ByteBufferReader& reader);
  std::vector<std::uint8_t> toBytes();

  static constexpr std::uint16_t SERVICE_ID = 0x020A;

private:
  std::uint8_t channel;
  std::uint8_t status;

};

}
