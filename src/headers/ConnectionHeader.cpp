
#include "ConnectionHeader.h"
#include "KnxStructure.h"

ConnectionHeader::ConnectionHeader(std::uint8_t channel, std::uint8_t sequence,
                                   std::uint8_t status)
    : KnxStructure{channel}, sequence{sequence}, status{status} {
    }

ConnectionHeader ConnectionHeader::parseAndCreate(ByteBufferReader &reader) {
  auto [length, type] = KnxStructure::parse(reader);
  return {type, 
                         reader.readUint8(), 
                         reader.readUint8()};
}

void ConnectionHeader::appendToByteArray(ByteBufferWriter &data) const {
  appendKnxStructure(data, 4);
  data.writeUint8(sequence);
  data.writeUint8(status);
}

std::uint8_t ConnectionHeader::getSequence() const { return sequence; }

std::uint8_t ConnectionHeader::getStatus() const { return status; }
std::uint8_t ConnectionHeader::getChannel() const { return getType(); }
// ConnectionHeader::
