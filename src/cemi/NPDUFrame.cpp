#include "NPDUFrame.h"


NPDUFrame::NPDUFrame(TCPI&& tcpi, DataACPI&& acpi) : tcpi{tcpi}, acpi{acpi}{
}

NPDUFrame NPDUFrame::createAndParse(ByteBufferReader& bytebuffer) {
  const byte length = bytebuffer.readUint8();
  const byte firstByte = bytebuffer.readUint8();
  return NPDUFrame{TCPI::parseAndCreate(firstByte & 0xF0), DataACPI::parseAndCreate(firstByte & 0x0F, length, bytebuffer)};
}

void NPDUFrame::toBytes(ByteBufferWriter& writer) {
  byte firstByte = tcpi.toByte();
  acpi.toBytes(firstByte, writer);
}

const TCPI& NPDUFrame::getTCPI() const {
  return tcpi;
}

const DataACPI& NPDUFrame::getACPI() const {
  return acpi;
}
