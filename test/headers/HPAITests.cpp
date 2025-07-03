#include "HPAI.h"
#include "bytes/ByteBufferReader.h"
#include <gtest/gtest.h>


TEST(HPAI, toByteArray) {
  HPAI hpai(IpAddress{}, 0xABCD, HPAI::UDP);
  auto answer = hpai.bodyAsByteArray();
  
  EXPECT_EQ(8, answer.size());

  EXPECT_EQ(0x08, answer[0]); // size
  EXPECT_EQ(0x01, answer[1]); // udp
  EXPECT_EQ(0x00, answer[2]); // ip
  EXPECT_EQ(0x00, answer[3]);
  EXPECT_EQ(0x00, answer[4]);
  EXPECT_EQ(0x00, answer[5]);
  EXPECT_EQ(0xAB, answer[6]); // port
  EXPECT_EQ(0xCD, answer[7]);

  ByteBufferReader byteBuffer{answer};
  HPAI fromArray = HPAI::createAndParse(byteBuffer);
  EXPECT_TRUE(fromArray.isUdp());
  EXPECT_EQ(0xABCD, fromArray.getPort());
}
