#pragma once

#include "HPAI.h"
#include <cstdint>
#include <vector>

class SearchRequest {
public:
  explicit SearchRequest(HPAI hpaiLocal);
  static SearchRequest newDefault();

  static constexpr std::array<std::uint8_t, 2> SERVICE_ID = {0x02, 0x01};

  std::vector<std::uint8_t> toBytes();

private:
  HPAI hpaiLocal;
};
