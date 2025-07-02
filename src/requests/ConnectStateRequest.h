#pragma once

#include "HPAI.h"
#include <cstdint>

class ConnectStateRequest {
public:

  static constexpr std::array<std::uint8_t, 2> SERVICE_ID = {0x02, 0x09};


private:
  std::uint8_t channelId;
  HPAI controlEndPoint;
};
