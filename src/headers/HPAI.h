#pragma once

#include "IpAddress.h"
#include "KnxStructure.h"
#include "bytes/ByteBufferReader.h"
#include <array>
#include <asio/awaitable.hpp>
#include <cstdint>

class HPAI : KnxStructure {
public:
  HPAI(){};
  HPAI(IpAddress address, int port, std::uint8_t protocolCode);

  std::array<std::uint8_t, 8> bodyAsByteArray();
  void appendToByteArray(std::vector<std::uint8_t>& data);

  static HPAI createAndParse(ByteBufferReader& byteBuffer);


  IpAddress getAddress() const;
  std::uint16_t getPort() const;
  std::uint8_t getProtocol() const;
  bool isUdp() const;
  bool isTcp() const;

  static constexpr uint8_t UDP = 0x01;
  static constexpr uint8_t TCP = 0x02;
protected:
  void parseBody(ByteBufferReader& byteBuffer, std::uint16_t length) override;
private:
  IpAddress address;
  std::uint16_t port;
};


