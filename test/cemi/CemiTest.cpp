#include "Cemi.h"
#include "ByteBufferReader.h"
#include <gtest/gtest.h>

static std::array<byte, 12> test_frame1 = {0x29, 0x00, 0xBC, 0xE0, 0x11, 0x09,
                                           0x2B, 0x08, 0x02, 0x00, 0x80, 0x00};

static std::array<byte, 15> test_frame2 = {0x29, 0x4,  0x1,  0x2,  0x0,
                                         0x6c, 0xbc, 0xd0, 0x11, 0x4,
                                         0x8,  0x3,  0x1,  0x0,  0x0};
TEST(Cemi, parse1) {
  ByteBufferReader byteBuffer{test_frame1};
  Cemi cemi = Cemi::parse(byteBuffer);
  ASSERT_EQ(1, cemi.getSource().getArea());
  ASSERT_EQ(1, cemi.getSource().getLine());
  ASSERT_EQ(9, cemi.getSource().getDevice());
}

TEST(Cemi, parse2) {
  ByteBufferReader byteBuffer{test_frame2};
  Cemi cemi = Cemi::parse(byteBuffer);
  ASSERT_EQ(0x29, cemi.getMessageCode());
  ASSERT_EQ(1, cemi.getSource().getArea());
  ASSERT_EQ(1, cemi.getSource().getLine());
  ASSERT_EQ(4, cemi.getSource().getDevice());
  auto destination = cemi.getDestination();
  bool isGroup = std::holds_alternative<GroupAddress>(destination);
  ASSERT_TRUE(isGroup);
  if(isGroup) {
    const auto group = std::get<GroupAddress>(destination);
    const GroupAddress expected{1,0,3};
    ASSERT_EQ(expected, group);
  }
  auto acpi = cemi.getNPDU().getACPI();
  ASSERT_EQ(DataACPI::GROUP_VALUE_READ, acpi.getType()); 
  
}

// only 4 bit data works, otherwise length wont be good
TEST(Cemi, write) {
  std::array<std::uint8_t, 2> value{0x00};
  value[1] = 0x01;
  GroupAddress ga{3,2,55};
  IndividualAddress source(0, 0, 0);
  Control control{KnxPrio::low, true};
  DataACPI dataAcpi{DataACPI::GROUP_VALUE_WRITE, value};
  TCPI tcpi{false, false, 0x00};
  NPDUFrame npduFrame{std::move(tcpi), std::move(dataAcpi)};
  Cemi cemi{Cemi::L_DATA_REQ, std::move(control), std::move(source),
              std::variant<IndividualAddress, GroupAddress>(ga),
              std::move(npduFrame)};
  std::vector<std::uint8_t> result;
  ByteBufferWriter writer{result};
  cemi.write(writer);
  ASSERT_EQ(11, result.size());
}

TEST(Cemi, write2) {
  std::array<std::uint8_t, 2> value{2};
  value[1] = 0x01;
  GroupAddress ga{3,2,55};
  IndividualAddress source(0, 0, 0);
  Control control{KnxPrio::low, true};
  DataACPI dataAcpi{DataACPI::GROUP_VALUE_WRITE, value};
  TCPI tcpi{false, false, 0x00};
  NPDUFrame npduFrame{std::move(tcpi), std::move(dataAcpi)};
  Cemi cemi{Cemi::L_DATA_REQ, std::move(control), std::move(source),
              std::variant<IndividualAddress, GroupAddress>(ga),
              std::move(npduFrame)};
  std::vector<std::uint8_t> result;
  ByteBufferWriter writer{result};
  cemi.write(writer);
  ASSERT_EQ(13, result.size());
}
