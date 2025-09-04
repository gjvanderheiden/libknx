#include "knx/datapointtypes/Format.h"
#include <gtest/gtest.h>

static std::array<byte, 0x02> test_data_float = {0x0C, 0x8B};
using namespace knx::datapoint;

TEST(FormatTest, float16) {
  double value = 23.84;
  auto data = KnxFloat16Format::toData(value);
  double answer = KnxFloat16Format::toValue(data);
  EXPECT_EQ(value, answer);
}
TEST(FormatTest, negativeFloat16) {
  double value = -23.84;
  auto data = KnxFloat16Format::toData(value);
  double answer = KnxFloat16Format::toValue(data);
  EXPECT_EQ(value, answer);
}
TEST(FormatTest, float16_testdata) {
  double answer = KnxFloat16Format::toValue(test_data_float);
  EXPECT_EQ(23.26, answer);
}

