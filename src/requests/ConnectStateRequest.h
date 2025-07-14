#pragma once

#include "HPAI.h"

#include "AbstractRequest.h"
#include <cstdint>

class ConnectStateRequest final : public AbstractRequest {
public:
  ConnectStateRequest(HPAI&& controlEndPoint, std::uint8_t channelId);
  static constexpr std::uint16_t SERVICE_ID = 0x0207;

  std::vector<std::uint8_t> toBytes();

private:
  std::uint8_t channelId{};
  HPAI controlEndPoint;
};
