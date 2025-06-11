#include "IpAddress.h"

IpAddress IpAddress::parse(ByteBuffer &byteBuffer) {
  IpAddress ipAddress{};
  byteBuffer.takeCopyToSpan(ipAddress.address);
  return ipAddress;
}

std::string IpAddress::asString() {
  return std::format("{}.{}.{}.{}", address[0], address[1],
                               address[2], address[3]);
}
