#pragma once

#include <array>
#include <cstdint>

class IpAddress {
public:
  std::array<uint8_t,4> address{0};
};
