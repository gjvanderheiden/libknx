#include "ConnectionRequestInformation.h"
#include "KnxStructure.h"

ConnectionRequestInformation::ConnectionRequestInformation(
    std::uint8_t connectionType, std::uint8_t layer)
    : KnxStructure{connectionType}, layer{layer} {}

ConnectionRequestInformation ConnectionRequestInformation::newTunneling() {
  return {TUNNELING_CONNECTION_TYPE, TUNNELING_LINK_LAYER}; 
}

ConnectionRequestInformation ConnectionRequestInformation::parse(ByteBufferReader& reader) {
  auto [length, type] = KnxStructure::parse(reader);
  return ConnectionRequestInformation{type, reader.readBoolFromByte()};
}

void ConnectionRequestInformation::write(ByteBufferWriter &writer) const {
  writeKnxStructure(writer, 4);
  writer.writeUint8(layer);
  writer.writeUint8(0x00);
}
