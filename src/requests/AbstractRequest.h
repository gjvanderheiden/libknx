#pragma once

#include <cstdint>
#include <vector>

class AbstractRequest {
public:
  virtual ~AbstractRequest() = default;

  virtual std::vector<std::uint8_t> toBytes() = 0;
};
