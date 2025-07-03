#include "Control.h"

Control Control::parse(ByteBufferReader& byteBuffer) {
  Control control;
  auto byte1 = byteBuffer.readUint8();
  auto byte2 = byteBuffer.readUint8();
  return control;
}

bool Control::isDestinationGroupAddress() const {
  return destinationAdresIsGroup;
}
