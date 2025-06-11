#pragma once

#include <asio/awaitable.hpp>
#include <cstdint>

class KnxStructure {
public:
  KnxStructure(){};
  KnxStructure(int type, std::uint8_t protocolCode);


private:
  std::uint16_t type;
  std::uint16_t length;
};
