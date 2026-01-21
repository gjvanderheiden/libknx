#include "knx/datapointtypes/Format.h"
#include <gtest/gtest.h>

static constexpr std::array<byte, 0x02> TEST_DATA_FLOAT = {0x0C, 0x8B};
using namespace knx::datapoint;

TEST(FormatTest, float16) {
  const constexpr double TEST_VALUE = 23.84;
  const auto data = KnxFloat16Format::toData(TEST_VALUE);
  const double answer = KnxFloat16Format::toValue(data);
  EXPECT_EQ(TEST_VALUE, answer);
}

TEST(FormatTest, negativeFloat16) {
  constexpr double TEST_VALUE = -23.84;
  const auto data = KnxFloat16Format::toData(TEST_VALUE);
  const double answer = KnxFloat16Format::toValue(data);
  EXPECT_EQ(TEST_VALUE, answer);
}

TEST(FormatTest, float16Testdata) {
  const double answer = KnxFloat16Format::toValue(TEST_DATA_FLOAT);
  EXPECT_EQ(23.26, answer);
}

