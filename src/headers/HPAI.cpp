#include "HPAI.h"
#include "IpAddress.h"
#include <cstdint>
#include <gtest/gtest.h>

HPAI::HPAI(IpAddress address, int port, std::uint8_t protocolCode)
    : address(std::move(address)), port(port) {
  this->setType(protocolCode);
}

void HPAI::appendToByteArray(ByteBufferWriter& data) {
  data.writeUint8(8);
  data.writeUint8(getProtocol());
  address.appendToByteArray(data);
  data.writeUint16(getPort());
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
