
#include "ConnectionHeader.h"
#include "KnxStructure.h"

ConnectionHeader::ConnectionHeader(std::uint8_t channel, std::uint8_t sequence,
                                   std::uint8_t status)
    : KnxStructure{channel}, sequence{sequence}, status{status} {}

ConnectionHeader ConnectionHeader::parse(ByteBufferReader &reader) {
  auto [length, type] = KnxStructure::parse(reader);
  return {type, reader.readUint8(), reader.readUint8()};
}

void ConnectionHeader::write(ByteBufferWriter &writer) const {
  writeKnxStructure(writer, 4);
  writer.writeUint8(sequence);
  writer.writeUint8(status);
}

std::uint8_t ConnectionHeader::getSequence() const { return sequence; }

std::uint8_t ConnectionHeader::getStatus() const { return status; }
std::uint8_t ConnectionHeader::getChannel() const { return getType(); }
