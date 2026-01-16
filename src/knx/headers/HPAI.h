#pragma once

#include "knx/headers/KnxStructure.h"
#include "knx/headers/IpAddress.h"
#include "knx/bytes/ByteBufferReader.h"
#include <asio/awaitable.hpp>
#include <cstdint>

class HPAI final : public  KnxStructure {
public:
  static constexpr uint8_t UDP = 0x01;
  static constexpr uint8_t TCP = 0x02;
  static constexpr uint8_t SIZE = 8;

public:
  HPAI() = delete;
  HPAI(IpAddress address, int port, std::uint8_t protocolCode);
  ~HPAI() override = default;

  HPAI(const HPAI &) = default;
  HPAI(HPAI &&) = default;
  HPAI &operator=(const HPAI &) = delete;
  HPAI &operator=(HPAI &&) = delete;

  void write(ByteBufferWriter& writer) const override;
  static HPAI parse(ByteBufferReader& reader);

  [[nodiscard]] IpAddress getAddress() const;
  [[nodiscard]] std::uint16_t getPort() const;
  [[nodiscard]] std::uint8_t getProtocol() const;
  [[nodiscard]] bool isUdp() const;
  [[nodiscard]] bool isTcp() const;

private:
  IpAddress address;
  std::uint16_t port{0};
};


