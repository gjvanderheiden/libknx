#include "KnxAddress.h"

KnxAddress::KnxAddress(std::uint8_t high, std::uint8_t middle, std::uint8_t low)
    : high{high}, middle{middle}, low{low} {}

std::uint8_t KnxAddress::getHigh() const { return high; }
std::uint8_t KnxAddress::getMiddle() const { return middle; }
std::uint8_t KnxAddress::getLow() const { return low; }
IndividualAddress::IndividualAddress(std::uint8_t high, std::uint8_t middle,
                                     std::uint8_t low)
    : KnxAddress{high, middle, low} {}

IndividualAddress IndividualAddress::createAndParse(ByteBufferReader &bytebuffer) {
  ByteSpan address = bytebuffer.readByteSpan(2);
  std::uint8_t area = (address[0] >> 4) & 0x0F;
  std::uint8_t line = address[0] & 0x0F;
  return {area, line, address[1]};
}

std::uint8_t IndividualAddress::getArea() const { return getHigh(); }
std::uint8_t IndividualAddress::getLine() const { return getMiddle(); }
std::uint8_t IndividualAddress::getDevice() const  { return getLow(); }

GroupAddress::GroupAddress(std::uint8_t high, std::uint8_t middle,
                           std::uint8_t low)
    : KnxAddress{high, middle, low} {}

GroupAddress GroupAddress::createAndParse(ByteBufferReader &bytebuffer) {
  ByteSpan address = bytebuffer.readByteSpan(2);
  std::uint8_t area = (address[0] >> 3) & 0x1F;
  std::uint8_t line = (address[0] >> 4) & 0x03;
  return {area, line, address[1]};
}
