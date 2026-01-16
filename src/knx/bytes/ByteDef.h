#pragma once

#include <cstdint>
#include <span>

using byte = std::uint8_t;
using ByteSpan = std::span<byte>;

constexpr int BYTE_SIZE = 8;
constexpr int TWO_BYTES_SIZE = 16;
constexpr int THREE_BYTES_SIZE = 24;


