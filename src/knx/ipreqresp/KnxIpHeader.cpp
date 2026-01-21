#include "KnxIpHeader.h"

KnxIpHeader::KnxIpHeader(const std::uint16_t serviceType, const std::uint16_t lengthInBytes)
    : serviceType{serviceType}, lengthInBytes{lengthInBytes} {}

void KnxIpHeader::write(ByteBufferWriter &writer) const {
  write(writer, serviceType, lengthInBytes);
}

void KnxIpHeader::write(ByteBufferWriter& writer, std::uint16_t serviceType, std::uint16_t lengthInBytes) {
  writer.writeUint8(SIZE_HEADER);
  writer.writeUint8(PROTOCOL_VERSION);
  writer.writeUint16(serviceType);
  writer.writeUint16(lengthInBytes);
}

KnxIpHeader KnxIpHeader::parse(ByteBufferReader &reader) {
  // 0x06 Constant size of KNXnet/IP as defined in protocol version 1.0
  // 0x10 KNXnet/IP protocol version 1.0
  reader.skip(2); // should be a constant 2 bytes
  return KnxIpHeader{reader.readUint16(), reader.readUint16()};
}

std::uint16_t KnxIpHeader::getServiceType() const { return serviceType; }
std::uint16_t KnxIpHeader::getLengthInBytes() const { return lengthInBytes; }
