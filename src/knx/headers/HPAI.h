#pragma once

#include "knx/headers/KnxStructure.h"
#include "knx/headers/IpAddress.h"
#include "knx/bytes/ByteBufferReader.h"
#include <asio/awaitable.hpp>
#include <cstdint>

class HPAI final: KnxStructure {
public:
  static constexpr uint8_t UDP = 0x01;
  static constexpr uint8_t TCP = 0x02;
  static constexpr uint8_t SIZE = 8;

public:
  HPAI(IpAddress address, int port, std::uint8_t protocolCode);

  void write(ByteBufferWriter& writer) const override;
  static HPAI parse(ByteBufferReader& reader);

  [[nodiscard]] IpAddress getAddress() const;
  [[nodiscard]] std::uint16_t getPort() const;
  [[nodiscard]] std::uint8_t getProtocol() const;
  [[nodiscard]] bool isUdp() const;
  [[nodiscard]] bool isTcp() const;

private:
  IpAddress address;
  std::uint16_t port;
};


