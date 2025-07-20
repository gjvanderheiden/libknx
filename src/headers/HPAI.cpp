#include "HPAI.h"
#include "IpAddress.h"
#include "KnxStructure.h"
#include <cstdint>

HPAI::HPAI(IpAddress address, int port, std::uint8_t protocolCode)
    : KnxStructure(protocolCode), address(std::move(address)), port(port) {
}

void HPAI::appendToByteArray(ByteBufferWriter& data) const {
  data.writeUint8(8);
  data.writeUint8(getProtocol());
  address.appendToByteArray(data);
  data.writeUint16(getPort());
}

HPAI HPAI::createAndParse(ByteBufferReader& reader) {
  auto [length, type] = KnxStructure::parse(reader);
  return HPAI{IpAddress::parse(reader), reader.readUint16(), type};
}

IpAddress HPAI::getAddress() const { return this->address; }

std::uint16_t HPAI::getPort() const { return this->port; }

std::uint8_t HPAI::getProtocol() const { return this->getType(); }

bool HPAI::isUdp() const { return this->getProtocol() == UDP; }

bool HPAI::isTcp() const { return this->getProtocol() == TCP; }
