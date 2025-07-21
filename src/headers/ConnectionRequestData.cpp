
#include "ConnectionRequestData.h"
#include "KnxAddress.h"


ConnectionRequestData::ConnectionRequestData(std::uint8_t type, IndividualAddress&& address) : KnxStructure{type}, address{address}{};

ConnectionRequestData ConnectionRequestData::parse(ByteBufferReader &reader) {
  auto [length, type] = KnxStructure::parse(reader);
  return ConnectionRequestData{type, IndividualAddress::parse(reader)};
}

void ConnectionRequestData::write(ByteBufferWriter &data) const {
  writeKnxStructure(data, 4);
  address.write(data);
}

const IndividualAddress& ConnectionRequestData::getAddress() const {
  return address;
}

ConnectionRequestData ConnectionRequestData::newTunneling() {
  return {TUNNELING_TYPE, IndividualAddress{0,0,0}};
}
