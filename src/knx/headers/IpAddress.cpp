#include "IpAddress.h"
#include <format>

IpAddress::IpAddress(const std::uint8_t byte1, const std::uint8_t byte2, const std::uint8_t byte3, const std::uint8_t byte4) : address  {byte1, byte2, byte3, byte4}{
}
IpAddress::IpAddress(std::array<const uint8_t, 4> &&asArray)
    : address{std::move(asArray)} {}

IpAddress IpAddress::parse(ByteBufferReader &reader) {
  return IpAddress{reader.get4BytesCopy()};
}

void IpAddress::write(ByteBufferWriter &writer) const {
  writer.writeByteSpan(address);
}

std::string IpAddress::asString() {
  return std::format("{}.{}.{}.{}", address[0], address[1], address[2],
                     address[3]);
}
