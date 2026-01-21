#include "knx/bytes/ByteBufferWriter.h"
#include <cstdint>
#include <gtest/gtest.h>

TEST(ByteBufferWriter, writeUint8) {
  const std::uint8_t testValue = 0x55;
  std::vector<byte> data;
  ByteBufferWriter byteBufferWriter{data}; 
  byteBufferWriter.writeUint8(testValue);
  ASSERT_EQ(1, data.size());
  ASSERT_EQ(testValue, data[0]);
}

TEST(ByteBufferWriter, writeUint16) {
  const std::uint16_t testValue = 0x010F;
  std::vector<byte> data;
  ByteBufferWriter byteBufferWriter{data};
  byteBufferWriter.writeUint16(testValue);
  ASSERT_EQ(2, data.size());
  ASSERT_EQ(0x01, data[0]);
  ASSERT_EQ(0x0F, data[1]);
}

TEST(ByteBufferWriter, writeUint32) {
  const std::uint32_t testValue = 0x010F8033;
  std::vector<byte> data;
  ByteBufferWriter byteBufferWriter{data};
  byteBufferWriter.writeUint32(testValue);
  ASSERT_EQ(4, data.size());
  ASSERT_EQ(0x01, data[0]);
  ASSERT_EQ(0x0F, data[1]);
  ASSERT_EQ(0x80, data[2]);
  ASSERT_EQ(0x33, data[3]);
}

TEST(ByteBufferWriter, writeBoolAsByteAndWriteZeros) {
  std::vector<byte> data;
  ByteBufferWriter byteBufferWriter{data};
  byteBufferWriter.writeBoolAsByte(true);
  byteBufferWriter.writeBoolAsByte(false);
  byteBufferWriter.writeZeros(3);
  byteBufferWriter.writeBoolAsByte(true);

  ASSERT_EQ(6, data.size());
  ASSERT_EQ(0x01, data[0]);
  ASSERT_EQ(0x00, data[1]);
  ASSERT_EQ(0x00, data[2]);
  ASSERT_EQ(0x00, data[3]);
  ASSERT_EQ(0x00, data[4]);
  ASSERT_EQ(0x01, data[5]);
}

TEST(ByteBufferWriter, writeString) {
  std::vector<byte> data;
  ByteBufferWriter byteBufferWriter{data};
  byteBufferWriter.writeString("konijn");
  ASSERT_EQ(6, data.size());

  ASSERT_EQ(107, data[0]);
  ASSERT_EQ(111, data[1]);
  ASSERT_EQ(110, data[2]);
  ASSERT_EQ(105, data[3]);
  ASSERT_EQ(106, data[4]);
  ASSERT_EQ(110, data[5]);
}

TEST(ByteBufferWriter, writeTerminatedString1) {
  std::vector<byte> data;
  ByteBufferWriter byteBufferWriter{data};
  byteBufferWriter.writeTerminatedString("konijn",6);
  ASSERT_EQ(6, data.size());

  ASSERT_EQ(107, data[0]);
  ASSERT_EQ(111, data[1]);
  ASSERT_EQ(110, data[2]);
  ASSERT_EQ(105, data[3]);
  ASSERT_EQ(106, data[4]);
  ASSERT_EQ(110, data[5]);
}

TEST(ByteBufferWriter, readTerminatedString2) {
  std::vector<byte> data;
  ByteBufferWriter byteBufferWriter{data};
  byteBufferWriter.writeTerminatedString("konijn",8);
  ASSERT_EQ(8, data.size());

  ASSERT_EQ(107, data[0]);
  ASSERT_EQ(111, data[1]);
  ASSERT_EQ(110, data[2]);
  ASSERT_EQ(105, data[3]);
  ASSERT_EQ(106, data[4]);
  ASSERT_EQ(110, data[5]);
  ASSERT_EQ(0, data[6]);
  ASSERT_EQ(0, data[7]);
}

TEST(ByteBufferWriter, readTerminatedString3) {
  std::vector<byte> data;
  ByteBufferWriter byteBufferWriter{data};
  byteBufferWriter.writeTerminatedString("konijn",7);
  ASSERT_EQ(7, data.size());

  ASSERT_EQ(107, data[0]);
  ASSERT_EQ(111, data[1]);
  ASSERT_EQ(110, data[2]);
  ASSERT_EQ(105, data[3]);
  ASSERT_EQ(106, data[4]);
  ASSERT_EQ(110, data[5]);
  ASSERT_EQ(0, data[6]);
}

TEST(ByteBufferWriter, writeByteSpan) {
  constexpr std::array<byte, 8> TEST_DATA = { 0x01, 0x00, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x01};
  std::vector<byte> data;
  ByteBufferWriter byteBufferWriter{data};
  byteBufferWriter.writeByteSpan(TEST_DATA);
  
  ASSERT_EQ(8, data.size());
  ASSERT_EQ(0x01, data[0]);
  ASSERT_EQ(0x00, data[1]);
  ASSERT_EQ(0xFF, data[2]);
  ASSERT_EQ(0xF0, data[3]);
  ASSERT_EQ(0x00, data[4]);
  ASSERT_EQ(0x00, data[5]);
  ASSERT_EQ(0x00, data[6]);
  ASSERT_EQ(0x01, data[7]);
}
