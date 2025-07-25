#include "KnxAddress.h"

KnxAddress::KnxAddress(std::uint8_t high, std::uint8_t middle, std::uint8_t low)
    : high{high}, middle{middle}, low{low} {}

std::uint8_t KnxAddress::getHigh() const { return high; }
std::uint8_t KnxAddress::getMiddle() const { return middle; }
std::uint8_t KnxAddress::getLow() const { return low; }
IndividualAddress::IndividualAddress(std::uint8_t high, std::uint8_t middle,
                                     std::uint8_t low)
    : KnxAddress{high, middle, low} {}

IndividualAddress IndividualAddress::parse(ByteBufferReader &reader) {
  ByteSpan address = reader.readByteSpan(2);
  std::uint8_t area = (address[0] >> 4) & 0x0F;
  std::uint8_t line = address[0] & 0x0F;
  return {area, line, address[1]};
}

void IndividualAddress::write(ByteBufferWriter& writer) const {
  writer.writeUint8(((getArea() << 4) & 0xF0) | (getLine() & 0x0F));
  writer.writeUint8(getDevice());
}

std::uint8_t IndividualAddress::getArea() const { return getHigh(); }
std::uint8_t IndividualAddress::getLine() const { return getMiddle(); }
std::uint8_t IndividualAddress::getDevice() const  { return getLow(); }

GroupAddress::GroupAddress(std::uint8_t high, std::uint8_t middle,
                           std::uint8_t low)
    : KnxAddress{high, middle, low} {}

GroupAddress GroupAddress::parse(ByteBufferReader &reader) {
  ByteSpan address = reader.readByteSpan(2);
  std::uint8_t high = (address[0] >> 3) & 0x1F;
  std::uint8_t middle = address[0]  & 0x03;
  return {high, middle, address[1]};
}

void GroupAddress::toBytes(ByteBufferWriter& writer) const{
  writer.writeUint8(((getHigh() & 0x1F) << 3) | (getMiddle() & 0x1F));
  writer.writeUint8(getLow());
}

bool GroupAddress::operator==(const GroupAddress& otherGroupAddress) const {
  return this->getHigh() == otherGroupAddress.getHigh() &&
         this->getMiddle() == otherGroupAddress.getMiddle() &&
         this->getLow() == otherGroupAddress.getLow();
}
std::ostream& operator<<(std::ostream &stream, const GroupAddress& address) {
  return stream << (int)address.getHigh() << "/" << (int)address.getMiddle() << "/"
         << (int)address.getLow();
}

std::ostream& operator<<(std::ostream &stream, const IndividualAddress& address) {
  return stream << (int)address.getHigh() << "." << (int)address.getMiddle() << "."
         << (int)address.getLow();
}
