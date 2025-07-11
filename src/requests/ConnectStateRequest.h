#pragma once

#include "HPAI.h"

#include "AbstractRequest.h"

class ConnectStateRequest final : AbstractRequest {
public:
  static constexpr std::uint16_t SERVICE_ID = 0x0209;


private:
  std::uint8_t channelId{};
  HPAI controlEndPoint;
};
