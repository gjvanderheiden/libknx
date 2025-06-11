#pragma once

#include <array>
#include <asio/awaitable.hpp>
#include <cstdint>

class ConnectionRequestInformation {
public:

  std::array<std::uint8_t, 8> bodyAsByteArray();


private:
};


