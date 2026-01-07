#pragma once

#include "knx/headers/HPAI.h"

#include "AbstractRequest.h"
#include <cstdint>

class ConnectStateRequest final : public AbstractRequest {
public:
  static constexpr std::uint16_t SERVICE_ID = 0x0207;

public:
  ConnectStateRequest(HPAI &&controlEndPoint, const std::uint8_t channelId);

  std::vector<std::uint8_t> toBytes() override;

private:
  const HPAI controlEndPoint;
  const std::uint8_t channelId;
};
