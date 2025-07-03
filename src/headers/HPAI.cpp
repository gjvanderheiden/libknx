#include "HPAI.h"
#include "IpAddress.h"
#include <cstdint>
#include <gtest/gtest.h>

HPAI::HPAI(IpAddress address, int port, std::uint8_t protocolCode)
    : address(std::move(address)), port(port) {
  this->setType(protocolCode);
}

std::array<std::uint8_t, 8> HPAI::bodyAsByteArray() {
  std::array<std::uint8_t, 8> body;
  body[0] = 8;
  body[1] = this->getProtocol();
  std::copy(address.address.begin(), address.address.end(), body.begin() + 2);
  body[6] = (getPort() >> 8) & 0xFF;
  body[7] = getPort() & 0xFF;
  return body;
}

void HPAI::appendToByteArray(std::vector<std::uint8_t>& data) {
  data.push_back(8);
  data.push_back(this->getProtocol());
  address.addToByteArray(data);
  data.push_back((getPort() >> 8) & 0xFF);
  data.push_back(getPort() & 0xFF);
}

HPAI HPAI::createAndParse(ByteBufferReader& byteBuffer) {
  HPAI hpai;
  hpai.parse(byteBuffer);
  return hpai;
}

void HPAI::parseBody(ByteBufferReader& byteBuffer, std::uint16_t size) {
  address = IpAddress::parse(byteBuffer);
  port = byteBuffer.readUint16();
}

IpAddress HPAI::getAddress() const { return this->address; }

std::uint16_t HPAI::getPort() const { return this->port; }

std::uint8_t HPAI::getProtocol() const { return this->getType(); }

bool HPAI::isUdp() const { return this->getProtocol() == UDP; }

bool HPAI::isTcp() const { return this->getProtocol() == TCP; }
