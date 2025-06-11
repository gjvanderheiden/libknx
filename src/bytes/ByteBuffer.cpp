#include "ByteBuffer.h"
#include <cstdint>
#include <gtest/gtest.h>


ByteBuffer::ByteBuffer(std::span<byte> data) : data{data} {}

std::uint32_t ByteBuffer::takeUInt32BigEndian() { 
  if constexpr (ByteBuffer::isNativeLittleEndian()) {
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

void ByteBuffer::skip(int numberOfBytes) { index += numberOfBytes; }

std::string ByteBuffer::takeString(int numberOfBytes) { return "aap"; }

ByteSpan ByteBuffer::takeByteSpan(int numberOfBytes) {
  ByteSpan span = data.subspan(index, numberOfBytes);
  index += numberOfBytes;
  return span;
}

void ByteBuffer::takeCopyToSpan(std::span<byte> destination) {
  std::memcpy(destination.data(), data.data() + index, destination.size());
  index += destination.size();
}

bool ByteBuffer::takeBoolFromByte() {
  return data[index++] != 0;
}

std::uint8_t ByteBuffer::readUint8() {
  return data[index++];
}


std::uint16_t ByteBuffer::readUint16() {
  if constexpr (ByteBuffer::isNativeLittleEndian()) {
    std::uint16_t value = data[index++] << 8;
    value |= data[index++];
    return value;
  }  else {
    std::uint16_t value = data[index++];
    value |= data[index++] << 8;
    return value;
  }
}
