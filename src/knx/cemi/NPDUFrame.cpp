#include "knx/cemi/NPDUFrame.h"


NPDUFrame::NPDUFrame(TCPI&& tcpi, DataACPI&& acpi) : tcpi{tcpi}, acpi{acpi}{
}

NPDUFrame NPDUFrame::parse(ByteBufferReader& bytebuffer) {
  const byte length = bytebuffer.readUint8();
  const byte firstByte = bytebuffer.readUint8();
  return NPDUFrame{TCPI::parse(firstByte & 0xF0), DataACPI::parse(firstByte & 0x0F, length, bytebuffer)};
}

void NPDUFrame::write(ByteBufferWriter& writer) const {
  byte firstByte = tcpi.toByte();
  acpi.write(firstByte, writer);
}

const TCPI& NPDUFrame::getTCPI() const {
  return tcpi;
}

const DataACPI& NPDUFrame::getACPI() const {
  return acpi;
}
