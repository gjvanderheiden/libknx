#include "knx/headers/KnxAddress.h"
#include <gtest/gtest.h>


TEST(GroupAddress, operators) {
  GroupAddress a{1,2,3};
  const GroupAddress b{1,2,3};
  GroupAddress c{3,3,3};
  GroupAddress d{3,4,3};
  GroupAddress e{3,2,3};
  GroupAddress f{3,3,2};
  ASSERT_EQ(a,b);
  ASSERT_EQ(a,b);
  ASSERT_TRUE(a==b);
  ASSERT_FALSE(a==c | a==d | a==e);
  ASSERT_FALSE(a < b);
  ASSERT_FALSE(a > b);
  ASSERT_TRUE(a < c);
  ASSERT_TRUE(c < d);
  ASSERT_FALSE(c < f);
  ASSERT_TRUE(f < c);
  ASSERT_FALSE(f > c);
  ASSERT_TRUE(c < d);
  ASSERT_FALSE(c > d);
}

TEST(GroupAddress, toBytes_then_parse) {
  GroupAddress gaOut{8,5,11};
  std::vector<std::uint8_t> bytes;
  ByteBufferWriter writer(bytes);
  gaOut.toBytes(writer);
  ByteBufferReader reader(bytes);
  GroupAddress gaIn = GroupAddress::parse(reader);
  ASSERT_TRUE(gaOut == gaIn);
}
