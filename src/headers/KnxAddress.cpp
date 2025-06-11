#include "KnxAddress.h"

IndividualAddress IndividualAddress::parse(ByteBuffer &bytebuffer) {
  IndividualAddress result{};
  bytebuffer.takeCopyToSpan(result.address);
  return result;
}

std::uint8_t IndividualAddress::getArea() {
  return (address[0] >> 4) & 0xF0;
}
std::uint8_t IndividualAddress::getLine() {
  return address[0] & 0xF0;
}
std::uint8_t IndividualAddress::getDevice() {
  return address[1];
}
