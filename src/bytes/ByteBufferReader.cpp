#include "ByteBufferReader.h"
#include <algorithm>
#include <bit>
#include <cstdint>
#include <iterator>
#include <cstring>
ByteBufferReader::ByteBufferReader(const std::span<byte> data) : data{data} {}

std::uint32_t ByteBufferReader::readUint32() {
  std::uint32_t value{0};
  memcpy(&value, data.data() + index, sizeof(std::uint32_t));
  index += sizeof(std::uint32_t);
  if constexpr (ByteBufferReader::isNativeLittleEndian()) {
    value = std::byteswap(value);
  }
  return value;
}

std::size_t ByteBufferReader::bytesLeft() {
  return data.size() - index - 1;
}
void ByteBufferReader::skip(const int numberOfBytes) { index += numberOfBytes; }

std::string ByteBufferReader::readString(const int numberOfBytes) {
  std::string aString;
  std::copy(data.data() + index, data.data() +index +numberOfBytes, std::back_inserter(aString));
  return aString;
}

std::string ByteBufferReader::readTerminatedString(const int maxLength) {
  std::string knxString;
  const std::span<std::uint8_t> knxText = readByteSpan(maxLength);
  int i = 0;
  for (; i < maxLength && knxText[i] != 0x00; i++) {
    knxString.push_back(knxText[i]);
  }
  skip(maxLength - i);
  return knxString;
}

ByteSpan ByteBufferReader::readByteSpan(const int numberOfBytes) {
  ByteSpan span = data.subspan(index, numberOfBytes);
  index += numberOfBytes;
  return span;
}

void ByteBufferReader::copyToSpan(std::span<byte> destination) {
  memcpy(destination.data(), data.data() + index, destination.size());
  index += destination.size();
}

bool ByteBufferReader::readBoolFromByte() { return data[index++] != 0; }

std::uint8_t ByteBufferReader::readUint8() { return data[index++]; }

std::uint16_t ByteBufferReader::readUint16() {
  if constexpr (ByteBufferReader::isNativeLittleEndian()) {
    std::uint16_t value = data[index++] << 8;
    value |= data[index++];
    return value;
  } else {
    std::uint16_t value = data[index++];
    value |= data[index++] << 8;
    return value;
  }
}
