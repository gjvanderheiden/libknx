#include "ByteBufferWriter.h"
#include <algorithm>
#include <bit>
#include <cstdint>
#include <gtest/gtest.h>
#include <iterator>

ByteBufferWriter::ByteBufferWriter(std::vector<byte> &data) : data{data} {}

void ByteBufferWriter::writeUint32(std::uint32_t value) {
  if constexpr (ByteBufferWriter::isNativeLittleEndian()) {
    value = std::byteswap(value);
  }
  data.push_back(value  & 0xFF);
  data.push_back((value >> 8) & 0xFF);
  data.push_back((value >> 16) & 0xFF);
  data.push_back((value >> 24) & 0xFF);
}

void ByteBufferWriter::writeZeros(int numberOfBytes) {
  for (int i = 0; i < numberOfBytes; i++) {
    data.push_back(0x00);
  }
}

void ByteBufferWriter::writeString(std::string_view aString) {
  std::copy(aString.begin(), aString.end(), std::back_inserter(data));
}

void ByteBufferWriter::writeTerminatedString(std::string_view aString,
                                             int maxLength) {
  writeString(aString);
  writeZeros(maxLength - aString.size());
}

void ByteBufferWriter::writeByteSpan(ByteSpan byteSpan) {
  std::copy(byteSpan.begin(), byteSpan.end(), std::back_inserter(data));
}

void ByteBufferWriter::writeBoolAsByte(bool value) { this->writeUint8(value); }

void ByteBufferWriter::writeUint8(std::uint8_t value) { data.push_back(value); }

void ByteBufferWriter::writeUint16(std::uint16_t value) {
  if constexpr (ByteBufferWriter::isNativeLittleEndian()) {
    value = std::byteswap(value);
  }
  data.push_back(value  & 0xFF);
  data.push_back((value >> 8) & 0xFF);
}
