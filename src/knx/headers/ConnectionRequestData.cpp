
#include "ConnectionRequestData.h"
#include "KnxAddress.h"


ConnectionRequestData::ConnectionRequestData(std::uint8_t type, IndividualAddress&& address) : KnxStructure{type}, address{std::move(address)}{};

ConnectionRequestData ConnectionRequestData::parse(ByteBufferReader &reader) {
  auto [length, type] = KnxStructure::parse(reader);
  return ConnectionRequestData{type, IndividualAddress::parse(reader)};
}

void ConnectionRequestData::write(ByteBufferWriter &writer) const {
  writeKnxStructure(writer, 4);
  address.write(writer);
}

const IndividualAddress& ConnectionRequestData::getAddress() const {
  return address;
}

ConnectionRequestData ConnectionRequestData::newTunneling() {
  return {TUNNELING_TYPE, IndividualAddress{0,0,0}};
}
