#include "Control.h"

Control::Control(const bool destinationAddressIsGroup) : destinationAddressIsGroup{destinationAddressIsGroup} {

}

Control Control::parse(ByteBufferReader& byteBuffer) {
  const auto byte1 = byteBuffer.readUint8();
  const auto byte2 = byteBuffer.readUint8();
  const bool isGroup = (byte2 & 0b10000000) != 0x00;
  return Control{isGroup};
}

void Control::toBytes(ByteBufferWriter &writer) const {
  writer.writeUint8(0xbc);
  writer.writeUint8(0xd0);

}

bool Control::isDestinationGroupAddress() const {
  return destinationAddressIsGroup;
}
