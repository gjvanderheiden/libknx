#include "bytes/ByteBufferReader.h"
#include <gtest/gtest.h>


TEST(ByteBuffer, readUint8) {
  std::uint8_t testValue = 255;
  ByteBufferReader byteBuffer{std::span{&testValue, 1}};
  ASSERT_EQ(testValue, byteBuffer.readUint8());
}

