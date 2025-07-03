#include "ByteBufferReader.h"
#include <cstdint>
#include <gtest/gtest.h>


ByteBufferReader::ByteBufferReader(std::span<byte> data) : data{data} {}

std::uint32_t ByteBufferReader::readUInt32BigEndian() { 
  if constexpr (ByteBufferReader::isNativeLittleEndian()) {
    std::uint16_t value = data[index++] << 16;
    value |= data[index++] << 8;
    value |= data[index++];
    return value;
  }  else {
    std::uint16_t value = data[index++];
    value |= data[index++] << 8;
    value |= data[index++] << 16;
    return value;
  }
}

void ByteBufferReader::skip(int numberOfBytes) { index += numberOfBytes; }

std::string ByteBufferReader::readString(int numberOfBytes) { return "aap"; }

ByteSpan ByteBufferReader::readByteSpan(int numberOfBytes) {
  ByteSpan span = data.subspan(index, numberOfBytes);
  index += numberOfBytes;
  return span;
}

void ByteBufferReader::copyToSpan(std::span<byte> destination) {
  memcpy(destination.data(), data.data() + index, destination.size());
  index += destination.size();
}

bool ByteBufferReader::readBoolFromByte() {
  return data[index++] != 0;
}

std::uint8_t ByteBufferReader::readUint8() {
  return data[index++];
}


std::uint16_t ByteBufferReader::readUint16() {
  if constexpr (ByteBufferReader::isNativeLittleEndian()) {
    std::uint16_t value = data[index++] << 8;
    value |= data[index++];
    return value;
  }  else {
    std::uint16_t value = data[index++];
    value |= data[index++] << 8;
    return value;
  }
}
