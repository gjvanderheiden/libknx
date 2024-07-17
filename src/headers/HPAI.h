#pragma once

#include "IpAddress.h"
#include <array>
#include <cstdint>
#include <span>

class HPAI {
public:
  HPAI(IpAddress address, int port, std::uint8_t protocolCode);

  std::array<std::uint8_t, 8> bodyAsByteArray();
  static HPAI fromArray(std::span<std::uint8_t> vector, int fromIndex);

  IpAddress getAddress() const;
  std::uint16_t getPort() const;
  std::uint8_t getProtocol() const;
  bool isUdp() const;
  bool isTcp() const;

  static constexpr uint8_t UDP = 0x01;
  static constexpr uint8_t TCP = 0x02;

private:
  IpAddress address;
  std::uint16_t port;
  std::uint8_t protocolCode = UDP;
};
