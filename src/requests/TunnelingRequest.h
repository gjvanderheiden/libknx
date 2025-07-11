#pragma once

#include "cemi/Cemi.h"
#include "headers/ConnectionHeader.h"
#include <vector>

#include "AbstractRequest.h"

class TunnelingRequest final : AbstractRequest {
public:
  static constexpr std::uint16_t SERVICE_ID = 0x0420;

public:
  explicit TunnelingRequest(Cemi&& cemi, ConnectionHeader&& connectionHeader);

  std::vector<std::uint8_t> toBytes();

private:
  Cemi cemi;
  ConnectionHeader connectionHeader;
};
