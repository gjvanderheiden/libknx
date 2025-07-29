#include "knx/bytes/ByteBufferReader.h"
#include "knx/requests/DisconnectRequest.h"
#include "knx/headers/KnxIpHeader.h"
#include <gtest/gtest.h>

static std::array<byte, 0x10> test_frame1 = {0x6, 0x10, 0x2, 0x9, 0x0,  0x10,
                                             0x1, 0x0,  0x8, 0x1, 0xc0, 0xa8,
                                             0xa, 0xf,  0xe, 0x57};

TEST(DisconnectRequest, parse1) {
  ByteBufferReader reader{test_frame1};
  KnxIpHeader header = KnxIpHeader::parse(reader);
  DisconnectRequest request = DisconnectRequest::parse(reader);
  ASSERT_EQ(1, request.getChannel());
  ASSERT_EQ(3671, request.getControlEndpoint().getPort());
}
