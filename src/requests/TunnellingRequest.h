#pragma once

#include "HPAI.h"
#include "cemi/Cemi.h"
#include "responses/TunnelAckResponse.h"
#include <cstdint>
#include <vector>

class SearchRequest {
public:
  explicit SearchRequest(HPAI hpaiLocal);
  static SearchRequest newDefault();

  static constexpr std::array<std::uint8_t, 2> SERVICE_ID = {0x04, 0x20};

  std::vector<std::uint8_t> toBytes();

private:
  Cemi cemi;
  ConnectionHeader connectionHeader;
};
