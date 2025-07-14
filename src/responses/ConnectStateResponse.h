#pragma once

#include "bytes/ByteBufferReader.h"
#include <cstdint>

namespace knx::requestresponse {

class ConnectStateResponse {
public:
  ConnectStateResponse(std::uint8_t channelId, std::uint8_t status);
  static ConnectStateResponse parse(ByteBufferReader& buffer);
  static constexpr std::uint16_t SERVICE_ID = 0x0208;
  
  std::uint8_t getChannelId() const;
  std::uint8_t getStatus() const;

private:
  const std::uint8_t channelId;
  const std::uint8_t status;
};

}
