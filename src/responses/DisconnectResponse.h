#pragma once

#include "ByteBufferReader.h"
#include <cstdint>
#include <vector>

namespace knx::requestresponse {

class DisconnectResponse {
public:
  static constexpr std::uint16_t SERVICE_ID = 0x020A;

public:
  DisconnectResponse(std::uint8_t channel, std::uint8_t status = 0);

  static DisconnectResponse parse(ByteBufferReader& reader);
  std::vector<std::uint8_t> toBytes();

private:
  std::uint8_t channel;
  std::uint8_t status;
};

}
