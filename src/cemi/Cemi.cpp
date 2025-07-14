#include "Cemi.h"
#include "KnxAddress.h"
#include "cemi/NPDUFrame.h"


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
  IndividualAddress source = IndividualAddress::createAndParse(byteBuffer);
  std::variant<IndividualAddress, GroupAddress> destination;
  if(control.isDestinationGroupAddress()) {
    destination = GroupAddress::createAndParse(byteBuffer);
  } else {
     destination = IndividualAddress::createAndParse(byteBuffer);
  }
  NPDUFrame npdu = NPDUFrame::createAndParse(byteBuffer);
  return {std::move(messageCode), std::move(control), std::move(source), std::move(destination), std::move(npdu)};
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
std::variant<IndividualAddress, GroupAddress>& Cemi::getDestination() {
  return destination;
}

const NPDUFrame& Cemi::getNPDU() const {
  return npdu;
}
