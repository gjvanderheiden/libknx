#include "IpAddress.h"
#include <format>

IpAddress::IpAddress(std::uint8_t byte1, std::uint8_t byte2, std::uint8_t byte3, std::uint8_t byte4) : address  {byte1, byte2, byte3, byte4}{
}
IpAddress::IpAddress(std::array<uint8_t, 4> &&asArray)
    : address{std::move(asArray)} {}

IpAddress IpAddress::parse(ByteBufferReader &byteBuffer) {
  std::array<std::uint8_t, 4> array;
  byteBuffer.copyToSpan(array);
  return {std::move(array)};
}

void IpAddress::write(ByteBufferWriter &data) const {
  data.writeUint8(address[0]);
  data.writeUint8(address[1]);
  data.writeUint8(address[2]);
  data.writeUint8(address[3]);
}

std::string IpAddress::asString() {
  return std::format("{}.{}.{}.{}", address[0], address[1], address[2],
                     address[3]);
}
