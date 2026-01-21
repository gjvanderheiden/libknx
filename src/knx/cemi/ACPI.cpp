#include "knx/cemi/ACPI.h"
#include <cstdint>

DataACPI::DataACPI(const std::uint8_t type, std::vector<byte>&& data, const bool fits6Bits) : type{type}, data{std::move(data)}, fits6Bits{fits6Bits}{}

DataACPI::DataACPI(const std::uint8_t type, const byte byteData, const bool fits6Bits):
  type{type}, data{byteData}, fits6Bits(fits6Bits){
}


DataACPI::DataACPI(const std::uint8_t type) : type{type}, fits6Bits{true} {}

DataACPI DataACPI::parse(const byte firstByte, const byte length,
                         ByteBufferReader &reader) {
  const byte secondByte = reader.readUint8();
  byte type = (firstByte & 0x03) << 2;
  type |= (secondByte & 0xC0) >> 6;
  std::vector<byte> data{};
  bool fits6Bits{false};
  if (length == 1) {
    data.push_back(secondByte & 0b00111111);
    fits6Bits = true;
  } else {
    data.resize(length - 1);
    reader.copyToSpan(data);
  }
  return DataACPI{type, std::move(data), fits6Bits};
}

void DataACPI::write(byte firstByte, const ByteBufferWriter &writer) const {
  const bool oneByteData = fits6Bits  && data.size() == 1 && (data[0] & 0b11000000) == 0;
  const bool noData = data.empty();
  writer.writeUint8(oneByteData || noData ? 1 : data.size() + 1); // length
  firstByte |= (type & 0x06) >> 2;
  writer.writeUint8(firstByte);
  byte secondByte = (type & 0x03) << 6;
  if (oneByteData) {
    secondByte |= data[0] & 0b0011111111;
    writer.writeUint8(secondByte);
  } else {
    writer.writeUint8(secondByte);
    writer.writeVector(data);
  }
}

std::uint8_t DataACPI::getType() const { return type; }

std::vector<uint8_t> DataACPI::getData() const { return data; }

ControlACPI::ControlACPI(const ControlType controlType)
    : controlType(controlType) {}

ControlType getType(const byte lastByte) {
  switch (lastByte & 0b11) {
  case 0b00:
    return ControlType::connect;
  case 0b01:
    return ControlType::disconnect;
  case 0b10:
    return ControlType::ack;
  case 0b11:
  default:
    return ControlType::nack;
  }
}

ControlACPI ControlACPI::parse(const byte firstByte, byte length,
                               ByteBufferReader &reader) {
  return ControlACPI{getType(firstByte)};
}
ControlType ControlACPI::getControlType() const { return controlType; }
