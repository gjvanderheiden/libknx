#pragma once

#include "HPAI.h"
#include <cstdint>
#include <vector>

class DisconnectRequest {
public:

  static constexpr std::array<std::uint8_t, 2> SERVICE_ID = {0x02, 0x09};

  std::vector<std::uint8_t> toBytes();

private:
  std::uint8_t channelId;
  HPAI controlEndpoint;
};
