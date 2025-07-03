#include "IpAddress.h"
#include <format>
#include <iterator>

IpAddress IpAddress::parse(ByteBufferReader &byteBuffer) {
  IpAddress ipAddress{};
  byteBuffer.copyToSpan(ipAddress.address);
  return ipAddress;
}

void IpAddress::addToByteArray(std::vector<std::uint8_t>& data) {
  std::copy(address.begin(), address.end(), std::back_inserter(data));
}

std::string IpAddress::asString() {
  return std::format("{}.{}.{}.{}", address[0], address[1],
                               address[2], address[3]);
}
