#include "knx/headers/KnxIpHeader.h"
#include "knx/bytes/ByteBufferReader.h"
#include "knx/cemi/ACPI.h"
#include "knx/requests/TunnelingRequest.h"
#include <gtest/gtest.h>

// TunnelReq #01:0 L_Data.ind 1.1.4->1/0/1 GroupValueWrite $00
static std::array<byte, 0x19> test_frame1 = {
    // knx ip header 
    0x06, 0x10, // knx ip header
    0x04, 0x20, // service type
    0x00, 0x19, // frame size
       // Tunnel Request:
           // ConnectionHeader
           0x04, 0x01, 0x00, 0x00,
           // Cemi
           0x29, // data type L_Data.ind
           0x04, 0x01, 0x02, 0x00, 0x6c,// additional info
           0xbc, // control byte 1: prio low
           0xd0, // control byte 2: hops 5
           0x11, 0x04, // source
           0x08, 0x01, // destination
           0x01, // length data
           0x00, // first 6 bits TCPI: Packet type : data, sequence type: unnumbered
                 // last 2 bits : ACPI type
           0x80  // first 2 bits: ACPI type, rest of the data
    };

// TunnelReq #01:1 L_Data.ind 1.1.4->1/0/1 GroupValueRead
static std::array<byte, 0x19> test_frame2 = {
    0x06, 0x10, 0x04, 0x20, 0x00, 0x19, 0x04, 0x01, 0x01,
    0x00, 0x29, 0x04, 0x01, 0x02, 0x00, 0x6c, 0xbc, 0xd0,
    0x11, 0x04, 0x08, 0x01, 0x01, 0x00, 0x00};

// TunnelReq #01:2 L_Data.ind 1.1.5->1/0/1 GroupValueResp $00
static std::array<byte, 0x19> test_frame3 = {
    0x06, 0x10, 0x04, 0x20, 0x00, 0x19, 0x04, 0x01, 0x02,
    0x00, 0x29, 0x04, 0x01, 0x02, 0x00, 0x6c, 0xbc, 0xd0,
    0x11, 0x05, 0x08, 0x01, 0x01, 0x00, 0x40};

// TunnelReq #01:6 L_Data.ind 1.1.4->0/0/0 SysNwkParamRead P=11 $01
static std::array<byte, 0x1E> test_frame4 = {
    0x06, 0x10, 0x04, 0x20, 0x00, 0x1e, 0x04, 0x01, 0x06,
    0x00, 0x29, 0x04, 0x01, 0x02, 0x00, 0x6c, 0xa0, 0xd0,
    0x11, 0x04, 0x00, 0x00, 0x06, 0x01, 0xc8, 0x00, 0x00,
    0x00, 0xb0, 0x01};

TEST(TunnelingRequest, parse1) {
  ByteBufferReader reader{test_frame1};
  KnxIpHeader header = KnxIpHeader::parse(reader);
  const TunnelRequest request = TunnelRequest::parse(reader);
  ASSERT_EQ(0, request.getConnectionHeader().getSequence());
  ASSERT_EQ(1, request.getConnectionHeader().getChannel());
  ASSERT_EQ(0, request.getConnectionHeader().getStatus());

  const Cemi &cemi = request.getCemi();
  ASSERT_FALSE(cemi.getNPDU().getTCPI().isControl());
  const auto& acpi = cemi.getNPDU().getACPI();
  ASSERT_EQ(DataACPI::GROUP_VALUE_WRITE, acpi.getType());
  auto data = acpi.getData();
  ASSERT_EQ(0x00, data[0]);
  ASSERT_EQ(0x00, data[1]);

}

TEST(TunnelingRequest, parse2) {
  ByteBufferReader reader{test_frame2};
  KnxIpHeader header = KnxIpHeader::parse(reader);
  const TunnelRequest request = TunnelRequest::parse(reader);
  ASSERT_EQ(1, request.getConnectionHeader().getSequence());
  ASSERT_EQ(1, request.getConnectionHeader().getChannel());
}

TEST(TunnelingRequest, parse3) {
  ByteBufferReader reader{test_frame3};
  KnxIpHeader header = KnxIpHeader::parse(reader);
  const TunnelRequest request = TunnelRequest::parse(reader);
  ASSERT_EQ(2, request.getConnectionHeader().getSequence());
  ASSERT_EQ(1, request.getConnectionHeader().getChannel());
}

TEST(TunnelingRequest, parse4) {
  ByteBufferReader reader{test_frame4};
  KnxIpHeader header = KnxIpHeader::parse(reader);
  const TunnelRequest request = TunnelRequest::parse(reader);
  ASSERT_EQ(6, request.getConnectionHeader().getSequence());
  ASSERT_EQ(1, request.getConnectionHeader().getChannel());
}
