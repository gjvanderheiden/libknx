#include "knx/cemi/Control.h"

Control::Control(KnxPrio prio, const bool destinationAddressIsGroup,
                 std::uint8_t hopCount)
    : priority{prio}, destinationAddressIsGroup{destinationAddressIsGroup},
      hopCount{hopCount} {}

Control Control::parse(ByteBufferReader &writer) {
  const auto byte1 = writer.readUint8();
  const auto byte2 = writer.readUint8();
  const bool isGroup = (byte2 & 0b10000000) != 0x00;
  const std::uint8_t hopCount = (byte2 >> 4) & 0x07;
  const KnxPrio prio = static_cast<KnxPrio>((byte1 >> 2) & 0x03);
  Control control = Control{prio, isGroup, hopCount};
  control.standardFrameType = (byte1 & 0b10000000) != 0x00;
  control.repeatOnError = (byte1 & 0b00100000) != 0x00;
  control.systemBroadcast = (byte1 & 0b00010000) != 0x00;
  control.acknowledgeWanted = (byte1 & 0b00000010) == 0x00;
  control.confirmationOnError = (byte1 & 0b00000001) == 0x00;
  return control;
}

void setBit(byte& byte, int pos, bool value) {
  if(value) {
    byte |= 0x01 << (pos - 1);
  }
}

void Control::write(ByteBufferWriter &writer) const {
  byte byte1 = 0x00;
  byte byte2 = 0x00;

  byte prioAsNumber = static_cast<byte>(getPrio());
  byte1 |= prioAsNumber << 2;

  setBit(byte1, 1, !confirmationOnErrorSet());
  setBit(byte1, 2, !isAcknowledgeWanted());
  setBit(byte1, 5, systemBroadcastSet());
  setBit(byte1, 6, repeatOnErrorSet());
  setBit(byte1, 8, isStandardFrameType());

  setBit(byte2, 8, isDestinationGroupAddress());
  byte2 |= getHopCount() << 4;

  writer.writeUint8(byte1);
  writer.writeUint8(byte2);
}


[[nodiscard]] bool Control::isStandardFrameType() const { return standardFrameType; }
[[nodiscard]] bool Control::repeatOnErrorSet() const { return repeatOnError; }
[[nodiscard]] bool Control::systemBroadcastSet() const {
  return systemBroadcast;
}
[[nodiscard]] KnxPrio Control::getPrio() const { return priority; }
[[nodiscard]] bool Control::isAcknowledgeWanted() const {return acknowledgeWanted;}
[[nodiscard]] bool Control::confirmationOnErrorSet() const{return confirmationOnError;}
[[nodiscard]] bool Control::isDestinationGroupAddress() const{return destinationAddressIsGroup;}
[[nodiscard]] std::uint8_t Control::getHopCount() const {return hopCount;}
