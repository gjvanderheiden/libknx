#pragma once

#include "ConnectionRequestData.h"
#include "HPAI.h"
#include "bytes/ByteBufferReader.h"
#include <cstdint>

namespace knx::requestresponse {

class ConnectResponse {
public:
  static ConnectResponse parse(ByteBufferReader &buffer);

  static constexpr std::uint16_t SERVICE_ID = 0x0206;

  std::uint8_t getChannelId() const;
  std::uint8_t getStatus() const;

private:
  std::uint8_t channelId{0};
  std::uint8_t status{0};
  HPAI dataEndPoint{};
  ConnectionRequestData crd{};
};

} // namespace knx::requestresponse
