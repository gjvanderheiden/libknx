#include "knx/bytes/ByteBufferReader.h"
#include <cstdint>
#include <gtest/gtest.h>


TEST(ByteBufferReader, readUint8) {
  std::uint8_t testValue = 255;
  ByteBufferReader byteBuffer{std::span{&testValue, 1}};
  ASSERT_EQ(testValue, byteBuffer.readUint8());
}

TEST(ByteBufferReader, readUint16) {
  std::array<byte, 4> testData = { 0x01, 0x0F, 0x80, 0x33};
  ByteBufferReader byteBuffer{testData};
  std::uint32_t result = byteBuffer.readUint16();
  ASSERT_EQ(0x010F, result);
  result = byteBuffer.readUint16();
  ASSERT_EQ(0x8033, result);
}

TEST(ByteBufferReader, readUint32) {
  std::array<byte, 4> testData = { 0x01, 0x0F, 0x80, 0x33};
  ByteBufferReader byteBuffer{testData};
  std::uint32_t result = byteBuffer.readUint32();
  ASSERT_EQ(0x010F8033, result);
}

TEST(ByteBufferReader, readBoolFromByteAndSkip) {
  std::array<byte, 8> testData = { 0x01, 0x00, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x01};
  ByteBufferReader byteBuffer{testData};
  ASSERT_TRUE(byteBuffer.readBoolFromByte());
  ASSERT_FALSE(byteBuffer.readBoolFromByte());
  ASSERT_TRUE(byteBuffer.readBoolFromByte());
  ASSERT_TRUE(byteBuffer.readBoolFromByte());
  ASSERT_FALSE(byteBuffer.readBoolFromByte());
  byteBuffer.skip(2);
  ASSERT_TRUE(byteBuffer.readBoolFromByte());
}

TEST(ByteBufferReader, readString) {
  std::array<byte, 6> testData = { 107, 111, 110, 105, 106, 110};
  ByteBufferReader byteBuffer{testData};
  ASSERT_EQ("konijn", byteBuffer.readString(6));
}

TEST(ByteBufferReader, readTerminatedString1) {
  std::array<byte, 6> testData = { 107, 111, 110, 105, 106, 110};
  ByteBufferReader byteBuffer{testData};
  ASSERT_EQ("konijn", byteBuffer.readTerminatedString(6));
}

TEST(ByteBufferReader, readTerminatedString2) {
  std::array<byte, 8> testData = { 107, 111, 110, 105, 106, 110, 0x00, 0x00};
  ByteBufferReader byteBuffer{testData};
  ASSERT_EQ("konijn", byteBuffer.readTerminatedString(8));
}

TEST(ByteBufferReader, readByteSpan) {
  std::array<byte, 8> testData = { 0x01, 0x00, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x01};
  ByteBufferReader byteBuffer{testData};
  auto result = byteBuffer.readByteSpan(2);
  ASSERT_EQ(2, result.size());
  ASSERT_EQ(0x01, result[0]);
  ASSERT_EQ(0x00, result[1]);
  // same pointer
  ASSERT_EQ(testData.data(), result.data());
  testData[1] = 0x55;
  ASSERT_EQ(0x01, result[0]);
  ASSERT_EQ(0x55, result[1]);
}
TEST(ByteBufferReader, copyToByteSpan) {
  std::array<byte, 8> testData = { 0x01, 0x00, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x01};
  ByteBufferReader byteBuffer{testData};
  std::array<byte, 2> resultArray;
  ByteSpan result{resultArray};
  byteBuffer.copyToSpan(result);
  ASSERT_EQ(2, result.size());
  ASSERT_EQ(0x01, result[0]);
  ASSERT_EQ(0x00, result[1]);
  testData[1] = 0x55;
  ASSERT_EQ(0x01, result[0]);
  ASSERT_EQ(0x00, result[1]);
}
