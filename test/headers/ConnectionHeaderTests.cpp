#include "ConnectionHeader.h"
#include "bytes/ByteBufferReader.h"
#include <gtest/gtest.h>

static std::array<byte, 0x04> test_frame1 = {0x4, 0x5, 0x01, 0x0};

TEST(ConnectionHeader, parse) {
  ByteBufferReader reader{test_frame1};
  ConnectionHeader connectionHeader = ConnectionHeader::parse(reader);
  ASSERT_EQ(5, connectionHeader.getChannel());
  ASSERT_EQ(1, connectionHeader.getSequence());
}
