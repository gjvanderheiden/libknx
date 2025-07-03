#include "cemi/Cemi.h"
#include "bytes/ByteBufferReader.h"
#include <gtest/gtest.h>

static std::array<byte, 12> test_frame1 = {
  0x29, 0x00, 0xBC, 0xE0, 0x11, 0x09, 0x2B, 0x08, 0x02, 0x00, 0x80, 0x00};


TEST(Cemi, createAndParse1) {
  ByteBufferReader byteBuffer{test_frame1};
  Cemi cemi = Cemi::parse(byteBuffer);
  ASSERT_EQ(1, cemi.getSource().getArea());
  ASSERT_EQ(1, cemi.getSource().getLine());
  ASSERT_EQ(9, cemi.getSource().getDevice());
}

