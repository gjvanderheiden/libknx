#include "knx/cemi/ACPI.h"
#include <cstdint>
#include <iterator>
#include <algorithm>

DataACPI::DataACPI(const std::uint8_t type, std::vector<byte>&& data) : type{type}, data{std::move(data)}{}

DataACPI::DataACPI(const std::uint8_t type, std::span<const std::uint8_t> data)
    : type{type}, data{} {
  this->data.reserve(data.size());
  std::ranges::copy(data, std::back_inserter(this->data));
}

DataACPI::DataACPI(const std::uint8_t type) : type{type}, data{} {}

DataACPI DataACPI::parse(const byte firstByte, const byte length,
                         ByteBufferReader &reader) {
  const byte secondByte = reader.readUint8();
  byte type = (firstByte & 0x03) << 2;
  type |= (secondByte & 0xC0) >> 6;
  std::vector<byte> data{};
  if (length == 1) {
    data.push_back(secondByte & 0b00111111);
  } else {
    data.resize(length - 1);
    reader.copyToSpan(data);
  }
  return DataACPI{type, std::move(data)};
}

void DataACPI::write(byte firstByte, ByteBufferWriter &writer) const {
  const bool oneByteData = data.size() == 1 && (data[0] & 0b11000000) == 0;
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

ControlACPI ControlACPI::parse(const byte firstByte, byte length,
                               ByteBufferReader &readerr) {
  return ControlACPI{getType(firstByte)};
}
ControlType ControlACPI::getControlType() const { return controlType; }
