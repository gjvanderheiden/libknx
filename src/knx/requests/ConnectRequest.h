#pragma once

#include "knx/requests/AbstractRequest.h"
#include "knx/headers/ConnectionRequestInformation.h"
#include "knx/headers/HPAI.h"

class ConnectRequest final : public AbstractRequest {
public:
  static constexpr std::uint16_t SERVICE_ID = 0x0205;
  static constexpr std::uint16_t SIZE = 26;

public:
  explicit ConnectRequest(HPAI&& controlEndPoint, HPAI&& dataEndPoint, ConnectionRequestInformation&& cri);

  std::vector<std::uint8_t> toBytes() override;

private:
  HPAI controlEndPoint;
  HPAI dataEndPoint;
  ConnectionRequestInformation cri;
};
