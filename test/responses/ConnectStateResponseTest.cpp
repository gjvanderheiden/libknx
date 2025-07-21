#include <gtest/gtest.h>
#include "KnxIpHeader.h"
#include "responses/ConnectStateResponse.h"
#include <array>

static std::array<byte,8> test_frame = {0x6, 0x10, 0x2, 0x8, 0x0, 0x8, 0x1, 0x0};

using namespace knx::requestresponse;


TEST(ConnectStateResponse, parse) {
  ByteBufferReader reader{test_frame};
  KnxIpHeader header = KnxIpHeader::parse(reader);
  ConnectStateResponse response = ConnectStateResponse::parse(reader);
  ASSERT_EQ(0, response.getStatus());
  ASSERT_EQ(1, response.getChannelId());
}  
