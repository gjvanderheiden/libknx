#include "knx/bytes/ByteBufferReader.h"
#include <algorithm>
#include <bit>
#include <cstdint>
#include <iterator>
#include <cstring>

ByteBufferReader::ByteBufferReader(const std::span<const byte> data) : data{data} {}

std::uint32_t ByteBufferReader::readUint32() {
  std::uint32_t value{0};
  memcpy(&value, data.subspan(index, sizeof(std::uint32_t)).data(), sizeof(std::uint32_t));
  index += sizeof(std::uint32_t);
  if constexpr (ByteBufferReader::isNativeLittleEndian()) {
    value = std::byteswap(value);
  }
  return value;
}

std::size_t ByteBufferReader::bytesLeft() const {
  return data.size() - index - 1;
}
void ByteBufferReader::skip(const int numberOfBytes) { index += numberOfBytes; }

std::string ByteBufferReader::readString(const int numberOfBytes) {
  std::string aString;
  std::copy_n(&data[index], numberOfBytes, std::back_inserter(aString));
  index += numberOfBytes;
  return aString;
}

std::string ByteBufferReader::readTerminatedString(const int maxLength) {
  std::string knxString;
  for (const auto byte: readByteSpan(maxLength)) {
    if(byte == 0x00) {
      break;
    }
    knxString.push_back(std::bit_cast<char>(byte));
  }
  return knxString;
}

std::array<const byte, 4> ByteBufferReader::get4BytesCopy() {
  return {data[index++], data[index++], data[index++], data[index++]};
}
std::span<const byte> ByteBufferReader::readByteSpan(const int numberOfBytes) {
  std::span<const byte> span = data.subspan(index, numberOfBytes);
  index += numberOfBytes;
  return span;
}

void ByteBufferReader::copyToSpan(std::span<byte> destination) {
  memcpy(destination.data(), data.subspan(index, destination.size()).data(), destination.size());
  index += destination.size();
}

bool ByteBufferReader::readBoolFromByte() { return data[index++] != 0; }

std::uint8_t ByteBufferReader::readUint8() { return data[index++]; }

std::int16_t ByteBufferReader::readInt16() {
  if  constexpr (ByteBufferReader::isNativeLittleEndian()) {
    auto value = static_cast<std::int16_t>(data[index++] << BYTE_SIZE);
    value =  static_cast<std::int16_t>(value | data[index++]);
    return value;
  } else {
    std::int16_t value = data[index++];
    value = static_cast<std::int16_t>(value | data[index++] << BYTE_SIZE);
    return value;
  }
}

std::uint16_t ByteBufferReader::readUint16() {
  if  constexpr (ByteBufferReader::isNativeLittleEndian()) {
    std::uint16_t value = data[index++] << BYTE_SIZE;
    value |= data[index++];
    return value;
  } else {
    std::uint16_t value = data[index++];
    value |= data[index++] << BYTE_SIZE;
    return value;
  }
}
