#pragma once

#include "ConnectionRequestInformation.h"
#include "HPAI.h"
#include <cstdint>

class ConnectRequest {
public:
  explicit ConnectRequest(HPAI&& controlEndPoint, HPAI&& dataEndPoint, ConnectionRequestInformation&& cri);

  static constexpr std::array<std::uint8_t, 2> SERVICE_ID = {0x02, 0x05};

  void sendTo() const;

private:
  HPAI controlEndPoint;
  HPAI dataEndPoint;
  ConnectionRequestInformation cri;
};
