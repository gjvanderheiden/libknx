#include "knx/cemi/Cemi.h"
#include "knx/headers/KnxAddress.h"
#include "NPDUFrame.h"


Cemi::Cemi(std::uint8_t messageCode, Control&& control, IndividualAddress&& source, std::variant<IndividualAddress, GroupAddress>&& destination, NPDUFrame&& frame) :
  messageCode{messageCode},
  control{std::move(control)},
  source{std::move(source)},
  destination{std::move(destination)},
  npdu{std::move(frame)} {
}

Cemi Cemi::parse(ByteBufferReader& byteBuffer) {
  std::uint8_t messageCode = byteBuffer.readUint8();
  std::uint8_t additionalInfoLength = byteBuffer.readUint8();
  byteBuffer.skip(additionalInfoLength);
  Control control = Control::parse(byteBuffer);
  IndividualAddress source = IndividualAddress::parse(byteBuffer);
  std::variant<IndividualAddress, GroupAddress> destination;
  if(control.isDestinationGroupAddress()) {
    destination = GroupAddress::parse(byteBuffer);
  } else {
     destination = IndividualAddress::parse(byteBuffer);
  }
  NPDUFrame npdu = NPDUFrame::parse(byteBuffer);
  return {std::move(messageCode), std::move(control), std::move(source), std::move(destination), std::move(npdu)};
}

void Cemi::write(ByteBufferWriter &writer) const {
  writer.writeUint8(messageCode);
  writer.writeUint8(0x00);
  control.write(writer);
  source.write(writer);
  if(std::holds_alternative<IndividualAddress>(destination)) {
    std::get<IndividualAddress>(destination).write(writer);
  } else {
    std::get<GroupAddress>(destination).toBytes(writer);
  }
  npdu.write(writer);
}

std::uint8_t Cemi::getMessageCode() const {
  return messageCode;
}

const Control& Cemi::getControl() const {
  return control;
}

const IndividualAddress& Cemi::getSource() const {
  return source;
}
const std::variant<IndividualAddress, GroupAddress>& Cemi::getDestination() const{
  return destination;
}

const NPDUFrame& Cemi::getNPDU() const {
  return npdu;
}
