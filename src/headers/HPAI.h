#pragma once

#include "IpAddress.h"
#include "KnxStructure.h"
#include "bytes/ByteBufferReader.h"
#include <asio/awaitable.hpp>
#include <cstdint>

class HPAI final: KnxStructure {
public:
  HPAI(){};
  HPAI(IpAddress address, int port, std::uint8_t protocolCode);


  void appendToByteArray(ByteBufferWriter& data) override;
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


