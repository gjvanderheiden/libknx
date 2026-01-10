#include "knx/bytes/ByteBufferWriter.h"
#include <algorithm>
#include <iterator>

ByteBufferWriter::ByteBufferWriter(std::vector<byte> &data) : data{data} {}

void ByteBufferWriter::writeUint32(std::uint32_t value) const {
  if constexpr (isNativeLittleEndian()) {
    value = std::byteswap(value);
  }
  data.push_back(value  & 0xFF);
  data.push_back((value >> 8) & 0xFF);
  data.push_back((value >> 16) & 0xFF);
  data.push_back((value >> 24) & 0xFF);
}

void ByteBufferWriter::writeZeros(const std::uint32_t numberOfBytes) const {
  for (int i = 0; i < numberOfBytes; i++) {
    data.push_back(0x00);
  }
}

void ByteBufferWriter::writeString(const std::string_view aString) const {
  std::copy(aString.begin(), aString.end(), std::back_inserter(data));
}

void ByteBufferWriter::writeTerminatedString(const std::string_view aString,
                                             const int maxLength) const {
  writeString(aString);
  writeZeros(maxLength - static_cast<std::uint32_t>(aString.size()));
}

void ByteBufferWriter::writeByteSpan(const std::span<const std::uint8_t> byteSpan) const {
  std::copy(byteSpan.begin(), byteSpan.end(), std::back_inserter(data));
}
void ByteBufferWriter::writeByteSpan(const ByteSpan byteSpan) const {
  std::copy(byteSpan.begin(), byteSpan.end(), std::back_inserter(data));
}

void ByteBufferWriter::writeVector(const std::vector<uint8_t>& vector) const {
  std::ranges::copy(vector, std::back_inserter(data));
}
void ByteBufferWriter::writeBoolAsByte(const bool value) const { this->writeUint8(value); }

void ByteBufferWriter::writeUint8(const std::uint8_t value) const { data.push_back(value); }

void ByteBufferWriter::writeUint16(std::uint16_t value) const {
  if constexpr (ByteBufferWriter::isNativeLittleEndian()) {
    value = std::byteswap(value);
  }
  data.push_back(value  & 0xFF);
  data.push_back((value >> 8) & 0xFF);
}

void ByteBufferWriter::writeInt16(std::int16_t value) const {
  if constexpr (ByteBufferWriter::isNativeLittleEndian()) {
    value = std::byteswap(value);
  }
  data.push_back(value  & 0xFF);
  data.push_back((value >> 8) & 0xFF);
}
