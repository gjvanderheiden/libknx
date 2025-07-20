
#include "ConnectionRequestData.h"
#include "KnxAddress.h"


ConnectionRequestData::ConnectionRequestData(std::uint8_t type, IndividualAddress&& address) : KnxStructure{type}, address{address}{};

ConnectionRequestData ConnectionRequestData::createAndParse(ByteBufferReader &data) {
  auto [length, type] = KnxStructure::parse(data);
  return ConnectionRequestData{type, IndividualAddress::createAndParse(data)};
}

void ConnectionRequestData::appendToByteArray(ByteBufferWriter &data) const {
  appendKnxStructure(data, 2);
}

const IndividualAddress& ConnectionRequestData::getAddress() const {
  return address;
}

ConnectionRequestData ConnectionRequestData::newTunneling() {
  return {TUNNELING_TYPE, IndividualAddress{0,0,0}};
}
