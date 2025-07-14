
#include "ConnectionHeader.h"
#include "KnxStructure.h"

ConnectionHeader::ConnectionHeader(std::uint8_t channel, std::uint8_t sequence,
                                   std::uint8_t status)
    : KnxStructure{channel}, sequence{sequence}, status{status} {}

void ConnectionHeader::parseBody(ByteBufferReader &reader,
                                 std::uint16_t length) {
  sequence = reader.readUint8();
  status = reader.readUint8();
}

void ConnectionHeader::appendToByteArray(ByteBufferWriter &data) {
  data.writeUint8(4);
  data.writeUint8(getType());
  data.writeUint8(sequence);
  data.writeUint8(status);
}

std::uint8_t ConnectionHeader::getSeqeunce() const { return sequence; }

std::uint8_t ConnectionHeader::getStatus() const { return status; }
std::uint8_t ConnectionHeader::getChannel() const { return getType(); }
// ConnectionHeader::
