#include "IpAddress.h"
#include <format>

IpAddress IpAddress::parse(ByteBufferReader &byteBuffer) {
  IpAddress ipAddress{};
  byteBuffer.copyToSpan(ipAddress.address);
  return ipAddress;
}

void IpAddress::appendToByteArray(ByteBufferWriter& data) {
  data.writeByteSpan(address);
}

std::string IpAddress::asString() {
  return std::format("{}.{}.{}.{}", address[0], address[1],
                               address[2], address[3]);
}
