#pragma once

#include "IpAddress.h"
#include "bytes/ByteBuffer.h"
#include <array>
#include <asio/awaitable.hpp>
#include <cstdint>

class HPAI {
public:
  HPAI(){};
  HPAI(IpAddress address, int port, std::uint8_t protocolCode);

  std::array<std::uint8_t, 8> bodyAsByteArray();

  template <typename S>
  static asio::awaitable<HPAI> readFromStream(StreamReader<S>& byteBuffer);


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

template <typename S>
asio::awaitable<HPAI> HPAI::readFromStream(StreamReader<S>& byteBuffer) {
  uint8_t size = co_await byteBuffer.readUint8();
  uint8_t protocolCode = co_await byteBuffer.readUint8();
  IpAddress ipAddress = co_await IpAddress::parse(byteBuffer);
  uint16_t port = co_await byteBuffer.readUint16();
  co_return HPAI{ipAddress, port, protocolCode};
}

