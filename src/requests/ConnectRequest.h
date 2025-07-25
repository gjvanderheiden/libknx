#pragma once

#include "AbstractRequest.h"
#include "ConnectionRequestInformation.h"
#include "HPAI.h"

class ConnectRequest final : public AbstractRequest {
public:
  static constexpr std::uint16_t SERVICE_ID = 0x0205;

  explicit ConnectRequest(HPAI&& controlEndPoint, HPAI&& dataEndPoint, ConnectionRequestInformation&& cri);

  std::vector<std::uint8_t> toBytes() override;


private:
  HPAI controlEndPoint;
  HPAI dataEndPoint;
  ConnectionRequestInformation cri;
};
