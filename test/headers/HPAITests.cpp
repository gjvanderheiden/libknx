#include "knx/headers/HPAI.h"
#include "knx/bytes/ByteBufferReader.h"
#include "knx/bytes/ByteBufferWriter.h"
#include <gtest/gtest.h>


TEST(HPAI, write) {
  HPAI hpai(IpAddress{1,2,3,4}, 0xABCD, HPAI::UDP);
  std::vector<byte> answer;
  ByteBufferWriter writer{answer};
  hpai.write(writer);
  
  EXPECT_EQ(8, answer.size());

  EXPECT_EQ(0x08, answer[0]); // size
  EXPECT_EQ(0x01, answer[1]); // udp
  EXPECT_EQ(0x01, answer[2]); // ip
  EXPECT_EQ(0x02, answer[3]);
  EXPECT_EQ(0x03, answer[4]);
  EXPECT_EQ(0x04, answer[5]);
  EXPECT_EQ(0xAB, answer[6]); // port
  EXPECT_EQ(0xCD, answer[7]);

  ByteBufferReader byteBuffer{answer};
  HPAI fromArray = HPAI::parse(byteBuffer);
  EXPECT_TRUE(fromArray.isUdp());
  EXPECT_EQ(0xABCD, fromArray.getPort());
}
