
#include "ConnectionRequestData.h"


ConnectionRequestData::ConnectionRequestData(std::uint8_t type) : KnxStructure(type){};

ConnectionRequestData ConnectionRequestData::createAndParse(ByteBufferReader &data) {
  auto [length, type] = KnxStructure::parse(data);
  data.skip(length);
  return ConnectionRequestData{type};
}

void ConnectionRequestData::appendToByteArray(ByteBufferWriter &data) const {
  appendKnxStructure(data, 2);
}

ConnectionRequestData ConnectionRequestData::newTunneling() {
  return {TUNNELING_TYPE};
}
