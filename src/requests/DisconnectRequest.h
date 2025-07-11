#pragma once

#include "AbstractRequest.h"
#include "HPAI.h"
#include <vector>

class DisconnectRequest final : AbstractRequest {
public:
  static constexpr std::uint16_t SERVICE_ID = 0x0209;

  std::vector<std::uint8_t> toBytes();

private:
  std::uint8_t channelId{0};
  HPAI controlEndpoint;
};
