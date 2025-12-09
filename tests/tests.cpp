#include <deaddev/bitmask.hpp>
#include <gtest/gtest.h>

enum simple_bitmask_flag_bits : uint16_t {
  SIMPLE_BITMASK_OPTION_0_BIT = 0x01,
  SIMPLE_BITMASK_OPTION_1_BIT = 0x04,
  SIMPLE_BITMASK_OPTION_2_BIT = 0x08,
  SIMPLE_BITMASK_OPTIONS_0_1 = SIMPLE_BITMASK_OPTION_0_BIT | SIMPLE_BITMASK_OPTION_1_BIT,
  SIMPLE_BITMASK_OPTIONS_1_2 = SIMPLE_BITMASK_OPTION_1_BIT | SIMPLE_BITMASK_OPTION_2_BIT,
  SIMPLE_BITMASK_OPTIONS_0_2 = SIMPLE_BITMASK_OPTION_0_BIT | SIMPLE_BITMASK_OPTION_2_BIT,
  SIMPLE_BITMASK_OPTIONS_0_1_2 = SIMPLE_BITMASK_OPTION_0_BIT |
                                 SIMPLE_BITMASK_OPTION_1_BIT |
                                 SIMPLE_BITMASK_OPTION_2_BIT,
};

DEADDEV_ENABLE_BITMASK(simple_bitmask_flag_bits, SIMPLE_BITMASK_OPTION_0_BIT,
                       SIMPLE_BITMASK_OPTION_1_BIT, SIMPLE_BITMASK_OPTION_2_BIT);

using simple_bitmask_flags = deaddev::bitmask<simple_bitmask_flag_bits>;

enum class scoped_bitmask_flag_bits : uint16_t {
  option_0_bit = 0x01,
  option_1_bit = 0x04,
  option_2_bit = 0x08,
  options_0_1 = option_0_bit | option_1_bit,
  options_1_2 = option_1_bit | option_2_bit,
  options_0_2 = option_0_bit | option_2_bit,
  options_0_1_2 = option_0_bit | option_1_bit | option_2_bit,
};
DEADDEV_ENABLE_BITMASK(scoped_bitmask_flag_bits, scoped_bitmask_flag_bits::option_0_bit,
                       scoped_bitmask_flag_bits::option_1_bit,
                       scoped_bitmask_flag_bits::option_2_bit);
using scoped_bitmask_flags = deaddev::bitmask<scoped_bitmask_flag_bits>;

TEST(simple_enum, init_from_int) {
  simple_bitmask_flags flags{0};
  ASSERT_EQ(flags, 0);
}

TEST(simple_enum, init_from_value) {
  simple_bitmask_flags flags{SIMPLE_BITMASK_OPTION_0_BIT};
  ASSERT_EQ(flags, SIMPLE_BITMASK_OPTION_0_BIT);
}

TEST(simple_enum, init_from_combination) {
  simple_bitmask_flags flags{SIMPLE_BITMASK_OPTION_1_BIT | SIMPLE_BITMASK_OPTION_2_BIT};
  ASSERT_EQ(flags, SIMPLE_BITMASK_OPTIONS_1_2);
}

TEST(simple_enum, bitwise_or) {
  simple_bitmask_flags flags{SIMPLE_BITMASK_OPTION_1_BIT | SIMPLE_BITMASK_OPTION_2_BIT};
  ASSERT_EQ(flags | SIMPLE_BITMASK_OPTION_0_BIT, SIMPLE_BITMASK_OPTIONS_0_1_2);
  flags |= SIMPLE_BITMASK_OPTION_0_BIT;
  ASSERT_EQ(flags, SIMPLE_BITMASK_OPTIONS_0_1_2);
}
TEST(simple_enum, bitwise_xor) {
  simple_bitmask_flags flags{SIMPLE_BITMASK_OPTION_1_BIT | SIMPLE_BITMASK_OPTION_2_BIT};
  ASSERT_EQ(flags ^ SIMPLE_BITMASK_OPTION_0_BIT, SIMPLE_BITMASK_OPTIONS_0_1_2);
  ASSERT_EQ(flags ^ SIMPLE_BITMASK_OPTION_1_BIT, SIMPLE_BITMASK_OPTION_2_BIT);
}

TEST(simple_enum, bitwise_and) {
  simple_bitmask_flags flags{SIMPLE_BITMASK_OPTION_1_BIT | SIMPLE_BITMASK_OPTION_2_BIT};
  ASSERT_EQ(flags & SIMPLE_BITMASK_OPTION_0_BIT, 0);
  ASSERT_EQ(flags & SIMPLE_BITMASK_OPTION_1_BIT, SIMPLE_BITMASK_OPTION_1_BIT);
}

TEST(simple_enum, bitwise_negation) {
  simple_bitmask_flags flags{SIMPLE_BITMASK_OPTION_1_BIT | SIMPLE_BITMASK_OPTION_2_BIT};
  ASSERT_EQ(~flags, SIMPLE_BITMASK_OPTION_0_BIT);
}

TEST(scoped_enum, init_from_int) {
  scoped_bitmask_flags flags{0};
  ASSERT_EQ(flags, 0);
}

TEST(scoped_enum, init_from_value) {
  scoped_bitmask_flags flags{scoped_bitmask_flag_bits::option_0_bit};
  ASSERT_EQ(flags, scoped_bitmask_flag_bits::option_0_bit);
}

TEST(scoped_enum, init_from_combination) {
  scoped_bitmask_flags flags{scoped_bitmask_flag_bits::option_1_bit |
                             scoped_bitmask_flag_bits::option_2_bit};
  ASSERT_EQ(flags, scoped_bitmask_flag_bits::options_1_2);
}

TEST(scoped_enum, bitwise_or) {
  scoped_bitmask_flags flags{scoped_bitmask_flag_bits::option_1_bit |
                             scoped_bitmask_flag_bits::option_2_bit};
  ASSERT_EQ(flags | scoped_bitmask_flag_bits::option_0_bit,
            scoped_bitmask_flag_bits::options_0_1_2);
  flags |= scoped_bitmask_flag_bits::option_0_bit;
  ASSERT_EQ(flags, scoped_bitmask_flag_bits::options_0_1_2);
}

TEST(scoped_enum, bitwise_xor) {
  scoped_bitmask_flags flags{scoped_bitmask_flag_bits::option_1_bit |
                             scoped_bitmask_flag_bits::option_2_bit};
  ASSERT_EQ(flags ^ scoped_bitmask_flag_bits::option_0_bit,
            scoped_bitmask_flag_bits::options_0_1_2);
  ASSERT_EQ(flags ^ scoped_bitmask_flag_bits::option_1_bit,
            scoped_bitmask_flag_bits::option_2_bit);
}

TEST(scoped_enum, bitwise_and) {
  scoped_bitmask_flags flags{scoped_bitmask_flag_bits::option_1_bit |
                             scoped_bitmask_flag_bits::option_2_bit};
  ASSERT_EQ(flags & scoped_bitmask_flag_bits::option_0_bit, 0);
  ASSERT_EQ(flags & scoped_bitmask_flag_bits::option_1_bit,
            scoped_bitmask_flag_bits::option_1_bit);
}

TEST(scoped_enum, bitwise_negation) {
  scoped_bitmask_flags flags{scoped_bitmask_flag_bits::option_1_bit |
                             scoped_bitmask_flag_bits::option_2_bit};
  ASSERT_EQ(~flags, scoped_bitmask_flag_bits::option_0_bit);
}