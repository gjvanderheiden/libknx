#include "KnxIpHeader.h"

KnxIpHeader::KnxIpHeader(std::uint16_t serviceType, std::uint16_t lengthInBytes)
    : serviceType{serviceType}, lengthInBytes{lengthInBytes} {}

void KnxIpHeader::write(ByteBufferWriter &writer) {
  writer.writeUint8(0x06);
  writer.writeUint8(0x10);
  writer.writeUint16(getServiceType());
  writer.writeUint16(lengthInBytes);
}
KnxIpHeader KnxIpHeader::parse(ByteBufferReader &reader) {
  // 0x06 Constant size of KNXnet/IP as defined in protocol version 1.0
  // 0x10 KNXnet/IP protocol version 1.0
  reader.skip(2);// should be a constant 2 bytes
  return {reader.readUint16(), reader.readUint16()};
}

std::uint16_t KnxIpHeader::getServiceType() { return serviceType; }
std::uint16_t KnxIpHeader::getLengthInBytes() { return lengthInBytes; }
