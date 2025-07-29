#include "knx/cemi/ACPI.h"
#include <cstdint>

DataACPI::DataACPI(const std::uint8_t type,
                   const std::array<std::uint8_t, 2> data)
    : type{type}, data{data} {}

DataACPI::DataACPI(const std::uint8_t type)
    : type{type}, data{0x00} {}

DataACPI DataACPI::parse(const byte firstByte, const byte length,
                         ByteBufferReader &reader) {
  const byte secondByte = reader.readUint8();
  byte type = (firstByte & 0x03) << 2;
  type |= (secondByte & 0xC0) >> 6;
  std::array<byte, 2> data{0};
  if (length == 1) {
    data[0] = 0x00;
    data[1] = secondByte & 0b00111111;
  } else if (length == 2) {
    data[1] = secondByte & 0b00111111;
    data[1] = reader.readUint8();
  }
  return DataACPI{type, data};
}

void DataACPI::write(byte firstByte, ByteBufferWriter &writer) const {
  bool oneByteData = (data[0] == 0) && ((data[1] & 0b11000000) == 0);
  writer.writeUint8(oneByteData ? 1 : 3); // length
  firstByte |= (type & 0x06) >> 2;
  writer.writeUint8(firstByte);
  byte secondByte = (type & 0x03) << 6;
  if (oneByteData) {
    secondByte |= data[1] & 0b0011111111;
    writer.writeUint8(secondByte);
  } else {
    writer.writeUint8(secondByte);
    writer.writeUint8(data[0]);
    writer.writeUint8(data[1]);
  }
}

std::uint8_t DataACPI::getType() const { return type; }

std::array<byte, 2> DataACPI::getData() const { return data; }

ControlACPI::ControlACPI(const ControlType controlType)
    : controlType(controlType) {}

ControlType getType(const byte lastByte) {
  switch (lastByte & 0b11) {
  case 0b00:
    return ControlType::Connect;
  case 0b01:
    return ControlType::Disconnect;
  case 0b10:
    return ControlType::Ack;
  case 0b11:
  default:
    return ControlType::Nack;
  }
}

ControlACPI ControlACPI::parse(byte firstByte, byte length,
                               ByteBufferReader &bytebuffer) {
  return ControlACPI{getType(firstByte)};
}
ControlType ControlACPI::getControlType() const { return controlType; }
