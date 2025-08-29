#include "knx/cemi/Control.h"

Control::Control(KnxPrio prio, const bool destinationAddressIsGroup,
                 std::uint8_t hopCount)
    : priority{prio}, destinationAddressIsGroup{destinationAddressIsGroup},
      hopCount{hopCount} {}

Control Control::parse(ByteBufferReader &writer) {
  const auto byte1 = writer.readUint8();
  const auto byte2 = writer.readUint8();
  const bool isGroup = (byte2 & 0b10000000) != 0x00;
  const KnxPrio prio = static_cast<KnxPrio>((byte1 >> 2) & 0x03);
  const std::uint8_t hopCount = (byte2 >> 4) & 0x05;
  Control control = Control{prio, isGroup, hopCount};
  control.standardFrameType = (byte1 & 0b10000000) != 0x00;
  control.repeatOnError = (byte1 & 0b00100000) != 0x00;
  control.systemBroadcast = (byte1 & 0b00010000) != 0x00;
  control.acknowledgeWanted = (byte1 & 0b00000010) == 0x00;
  control.confirmationOnError = (byte1 & 0b00000001) == 0x00;
  return control;
}

void Control::write(ByteBufferWriter &writer) const {
  writer.writeUint8(0xbc);
  writer.writeUint8(0xd0);
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
