#include "KnxIpHeader.h"

KnxIpHeader::KnxIpHeader(std::uint16_t serviceType, std::uint16_t lengthInBytes)
    : serviceType{serviceType}, lengthInBytes{lengthInBytes} {}

KnxIpHeader KnxIpHeader::createAndParse(ByteBufferReader &reader) {
  reader.skip(2);// should be a constant 2 bytes
  return {reader.readUint16(), reader.readUint16()};
}

std::uint16_t KnxIpHeader::getServiceType() { return serviceType; }
std::uint16_t KnxIpHeader::getLengthInBytes() { return lengthInBytes; }
