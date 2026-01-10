#include "knx/cemi/Cemi.h"
#include "NPDUFrame.h"
#include "knx/headers/KnxAddress.h"

Cemi::Cemi(const std::uint8_t messageCode, Control&& control,
           IndividualAddress &&source,
           std::variant<IndividualAddress, GroupAddress> &&destination,
           NPDUFrame &&frame)
    : messageCode{messageCode}, control{std::move(control)},
      source{std::move(source)}, destination{std::move(destination)},
      npdu{std::move(frame)} {}

Cemi::Cemi(const std::uint8_t messageCode,
           AdditionalInformation &&additionalInformation, Control &&control,
           IndividualAddress &&source,
           std::variant<IndividualAddress, GroupAddress> &&destination,
           NPDUFrame &&frame)
    : messageCode{messageCode},
      additionalInformation{std::move(additionalInformation)},
      control{std::move(control)}, source{std::move(source)},
      destination{std::move(destination)}, npdu{std::move(frame)} {}

Cemi Cemi::parse(ByteBufferReader &reader) {
  std::uint8_t messageCode = reader.readUint8();
  AdditionalInformation addInfo = AdditionalInformation::parse(reader);
  Control control = Control::parse(reader);
  IndividualAddress source = IndividualAddress::parse(reader);
  std::variant<IndividualAddress, GroupAddress> destination;
  if (control.isDestinationGroupAddress()) {
    destination = GroupAddress::parse(reader);
  } else {
    destination = IndividualAddress::parse(reader);
  }
  NPDUFrame npdu = NPDUFrame::parse(reader);
  return {messageCode, std::move(addInfo), std::move(control), std::move(source),
          std::move(destination), std::move(npdu)};
}

void Cemi::write(ByteBufferWriter &writer) const {
  writer.writeUint8(messageCode);
  additionalInformation.write(writer);
  control.write(writer);
  source.write(writer);
  if (std::holds_alternative<IndividualAddress>(destination)) {
    std::get<IndividualAddress>(destination).write(writer);
  } else {
    std::get<GroupAddress>(destination).toBytes(writer);
  }
  npdu.write(writer);
}

std::uint8_t Cemi::getMessageCode() const { return messageCode; }

const Control &Cemi::getControl() const { return control; }

const IndividualAddress &Cemi::getSource() const { return source; }

const std::variant<IndividualAddress, GroupAddress> &
Cemi::getDestination() const {
  return destination;
}

const NPDUFrame &Cemi::getNPDU() const { return npdu; }
