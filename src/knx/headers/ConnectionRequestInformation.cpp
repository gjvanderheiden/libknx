#include "ConnectionRequestInformation.h"
#include "KnxStructure.h"
#include <cstdint>

ConnectionRequestInformation::ConnectionRequestInformation(
    std::uint8_t connectionType, std::uint8_t layer)
    : KnxStructure{connectionType}, layer{layer} {}

ConnectionRequestInformation ConnectionRequestInformation::newTunneling() {
  return {TUNNELING_CONNECTION_TYPE, TUNNELING_LINK_LAYER}; 
}

ConnectionRequestInformation ConnectionRequestInformation::parse(ByteBufferReader& reader) {
  auto [length, type] = KnxStructure::parse(reader);
  std::uint8_t layer = reader.readUint8();
  reader.skip(1); // reserved
  return ConnectionRequestInformation{type, layer};
}

void ConnectionRequestInformation::write(ByteBufferWriter &writer) const {
  writeKnxStructure(writer, 4);
  writer.writeUint8(layer);
  writer.writeUint8(0x00);
}
