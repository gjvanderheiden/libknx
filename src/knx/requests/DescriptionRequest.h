#pragma once

#include "knx/requests/AbstractRequest.h"
#include "knx/headers/HPAI.h"

class DescriptionRequest final : public AbstractRequest {
public:
  static constexpr std::uint16_t SERVICE_ID = 0x0203;

  explicit DescriptionRequest(HPAI&& controlEndPoint);

  std::vector<std::uint8_t> toBytes() override;


private:
  HPAI controlEndPoint;
};
