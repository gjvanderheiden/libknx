
#include "KnxIpHeader.h"
#include "bytes/ByteBufferReader.h"
#include "requests/TunnelingRequest.h"
#include <gtest/gtest.h>
// TunnelReq #01:0 L_Data.ind 1.1.4->1/0/1 GroupValueWrite $00
static std::array<byte, 0x19> test_frame1 = {
    0x06, 0x10, 0x04, 0x20, 0x00, 0x19, 0x4, 0x1, 0x0, 0x0, 0x29, 0x4, 0x1,
    0x2,  0x0,  0x6c, 0xbc, 0xd0, 0x11, 0x4, 0x8, 0x1, 0x1, 0x0,  0x80};


// TunnelReq #01:1 L_Data.ind 1.1.4->1/0/1 GroupValueRead
static std::array<byte, 0x19> test_frame2 = {
    0x6, 0x10, 0x4,  0x20, 0x0,  0x19, 0x4, 0x1, 0x1, 0x0, 0x29, 0x4, 0x1,
    0x2, 0x0,  0x6c, 0xbc, 0xd0, 0x11, 0x4, 0x8, 0x1, 0x1, 0x0,  0x0};

// TunnelReq #01:2 L_Data.ind 1.1.5->1/0/1 GroupValueResp $00
static std::array<byte,0x19> test_frame3 = {0x6, 0x10, 0x4, 0x20, 0x0, 0x19, 0x4, 0x1, 0x2, 0x0, 0x29, 0x4, 0x1, 0x2, 0x0, 0x6c, 0xbc, 0xd0, 0x11, 0x5, 0x8, 0x1, 0x1, 0x0, 0x40};

// TunnelReq #01:6 L_Data.ind 1.1.4->0/0/0 SysNwkParamRead P=11 $01
static std::array<byte,0x1E> test_frame4 = {0x6, 0x10, 0x4, 0x20, 0x0, 0x1e, 0x4, 0x1, 0x6, 0x0, 0x29, 0x4, 0x1, 0x2, 0x0, 0x6c, 0xa0, 0xd0, 0x11, 0x4, 0x0, 0x0, 0x6, 0x1, 0xc8, 0x0, 0x0, 0x0, 0xb0, 0x1};

TEST(TunnelingRequest, createAndParse1) {
  ByteBufferReader reader{test_frame1};
  KnxIpHeader header = KnxIpHeader::createAndParse(reader);
  TunnelRequest request = TunnelRequest::parse(reader);
  ASSERT_EQ(0, request.getConnectionHeader().getSeqeunce());
  ASSERT_EQ(1, request.getConnectionHeader().getChannel());
}

TEST(TunnelingRequest, createAndParse2) {
  ByteBufferReader reader{test_frame2};
  KnxIpHeader header = KnxIpHeader::createAndParse(reader);
  TunnelRequest request = TunnelRequest::parse(reader);
  ASSERT_EQ(1, request.getConnectionHeader().getSeqeunce());
  ASSERT_EQ(1, request.getConnectionHeader().getChannel());
}

TEST(TunnelingRequest, createAndParse3) {
  ByteBufferReader reader{test_frame3};
  KnxIpHeader header = KnxIpHeader::createAndParse(reader);
  TunnelRequest request = TunnelRequest::parse(reader);
  ASSERT_EQ(2, request.getConnectionHeader().getSeqeunce());
  ASSERT_EQ(1, request.getConnectionHeader().getChannel());
}

TEST(TunnelingRequest, createAndParse4) {
  ByteBufferReader reader{test_frame4};
  KnxIpHeader header = KnxIpHeader::createAndParse(reader);
  TunnelRequest request = TunnelRequest::parse(reader);
  ASSERT_EQ(6, request.getConnectionHeader().getSeqeunce());
  ASSERT_EQ(1, request.getConnectionHeader().getChannel());
}
