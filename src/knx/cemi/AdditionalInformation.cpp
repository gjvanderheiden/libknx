#include "knx/cemi/AdditionalInformation.h"

AdditionalInformation AdditionalInformation::parse(ByteBufferReader &reader) {
  AdditionalInformation result;
  std::uint8_t remainingLength = reader.readUint8();
  while (remainingLength > 0) {
    AdditionalInformationType infoType =
        static_cast<AdditionalInformationType>(reader.readUint8());
    std::uint8_t elementLength = reader.readUint8();
    std::vector<byte> data;
    data.reserve(elementLength);
    data.resize(elementLength);
    reader.copyToSpan(data);
    result.elements.emplace_back(infoType, std::move(data));
    remainingLength -= elementLength + 2;
  }
  return result;
}

std::uint8_t AdditionalInformationElement::frameSize() const {
  return 2 + data.size();
}

void AdditionalInformationElement::write(ByteBufferWriter &writer) const {
  writer.writeUint8(static_cast<std::uint8_t>(type));
  writer.writeUint8(static_cast<std::uint8_t>(data.size()));
  writer.writeByteSpan(data);
}

void AdditionalInformation::write(ByteBufferWriter &writer) const {
  std::uint8_t totalLength = 0;
  for (auto const &element : this->elements) {
    totalLength += element.frameSize();
  }
  writer.writeUint8(totalLength);
  for (auto const &element : this->elements) {
    element.write(writer);
  }
};
