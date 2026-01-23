#include "knx/cemi/Control.h"

Control::Control(KnxPrio prio, const bool destinationAddressIsGroup,
                 std::uint8_t hopCount)
    : priority{prio}, destinationAddressIsGroup{destinationAddressIsGroup},
      hopCount{hopCount} {}

namespace{
bool isBitSet(const byte byte, const int pos) {
  return (byte & (0x01 << (pos - 1))) != 0x00;
}

bool isBitNotSet(const byte byte, const int pos) {
  return (byte & (0x01 << (pos - 1))) == 0x00;
}
void setBit(byte& byte, const int pos, const bool value) {
  if(value) {
    byte |= 0x01 << (pos - 1);
  }
}
}

Control Control::parse(ByteBufferReader &reader) {
  const auto byte1 = reader.readUint8();
  const auto byte2 = reader.readUint8();
  const bool isGroup = isBitSet(byte2, IS_GROUP_ADDRESS_POS);
  const std::uint8_t hopCount = (byte2 >> 4) & 0x07;
  const auto prio = static_cast<KnxPrio>((byte1 >> 2) & 0x03);
  Control control = Control{prio, isGroup, hopCount};
  control.standardFrameType = isBitSet(byte1, STANDARD_FRAME_TYPE_BIT_POS);
  control.repeatOnError = isBitSet(byte1, REPEAT_ON_ERROR_POS);
  control.systemBroadcast = isBitSet(byte1, SYSTEM_BROADCAST_POS);
  control.acknowledgeWanted = isBitNotSet(byte1, ACKNOWLEDGE_WANTED_POS);
  control.confirmationOnError = isBitNotSet(byte1, CONFIRMATION_ON_ERROR_POS);
  return control;
}

void Control::write(ByteBufferWriter &writer) const {
  byte byte1 = 0x00;
  byte byte2 = 0x00;

  byte prioAsNumber = static_cast<byte>(getPrio());
  byte1 |= prioAsNumber << 2;

  setBit(byte1, CONFIRMATION_ON_ERROR_POS, !confirmationOnErrorSet());
  setBit(byte1, ACKNOWLEDGE_WANTED_POS,  !isAcknowledgeWanted());
  setBit(byte1, SYSTEM_BROADCAST_POS, systemBroadcastSet());
  setBit(byte1, REPEAT_ON_ERROR_POS, repeatOnErrorSet());
  setBit(byte1, STANDARD_FRAME_TYPE_BIT_POS, isStandardFrameType());

  setBit(byte2, IS_GROUP_ADDRESS_POS, isDestinationGroupAddress());
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
