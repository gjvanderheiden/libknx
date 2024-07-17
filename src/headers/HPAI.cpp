#include "HPAI.h"
#include "IpAddress.h"
#include <cstdint>
#include <gtest/gtest.h>

HPAI::HPAI(IpAddress address, int port, std::uint8_t protocolCode)
    : address(std::move(address)), port(port) {}

std::array<std::uint8_t, 8> HPAI::bodyAsByteArray() {
  std::array<std::uint8_t, 8> body;
  body[0] = 8;
  body[1] = this->protocolCode;
  std::copy(address.address.begin(), address.address.end(), body.begin() + 2);
  body[6] = (getPort() >> 8) & 0xFF;
  body[7] = getPort()  & 0xFF;
  return body;
}

HPAI HPAI::fromArray(std::span<std::uint8_t> data, int fromIndex) {
  int index = fromIndex;
  uint8_t length = data[index++];
  if (length == 8) {
    uint8_t protocolCode = data[index++];
    IpAddress address;

    address.address[0] = data[index++];
    address.address[1] = data[index++];
    address.address[2] = data[index++];
    address.address[3] = data[index++];

    uint16_t port = data[index++ ] << 8;
    port |= data[index++] ;
    return HPAI(address, port, protocolCode);
  } else {
    throw std::exception();
  }
}

IpAddress HPAI::getAddress() const { return this->address; }

std::uint16_t HPAI::getPort() const { return this->port; }

std::uint8_t HPAI::getProtocol() const {
return this->protocolCode;
}

bool HPAI::isUdp() const {
  return this->protocolCode == UDP;
}
bool HPAI::isTcp() const {
  return this->protocolCode == TCP;
}
