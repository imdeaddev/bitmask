#include <cassert>
#include <deaddev/bitmask.hpp>

namespace ns {
enum class some_bitmask_flag_bits {
  flag_0_bit = 0x01,
  flag_1_bit = 0x02,
  flag_2_bit = 0x04,
  flag_3_bit = 0x08,
  flag_4_bit = 0x10,
  flag_5_bit = 0x20,
  flag_6_bit = 0x40,
  flag_7_bit = 0x80,
};

DEADDEV_ENABLE_BITMASK(some_bitmask_flag_bits,
                       some_bitmask_flag_bits::flag_0_bit, // [optional] all flags list
                       some_bitmask_flag_bits::flag_1_bit,
                       some_bitmask_flag_bits::flag_2_bit,
                       some_bitmask_flag_bits::flag_3_bit,
                       some_bitmask_flag_bits::flag_4_bit,
                       some_bitmask_flag_bits::flag_5_bit,
                       some_bitmask_flag_bits::flag_6_bit,
                       some_bitmask_flag_bits::flag_7_bit);
using some_bitmask_flags = deaddev::bitmask<some_bitmask_flag_bits>;

} // namespace ns

//
// alternative variant without namespace access:
// DEADDEV_ENABLE_BITMASK_EXTERNAL(ns::some_bitmask_flag_bits,
//                                 ns::some_bitmask_flag_bits::flag_0_bit,
//                                 ns::some_bitmask_flag_bits::flag_1_bit,
//                                 ns::some_bitmask_flag_bits::flag_2_bit,
//                                 ns::some_bitmask_flag_bits::flag_3_bit,
//                                 ns::some_bitmask_flag_bits::flag_4_bit,
//                                 ns::some_bitmask_flag_bits::flag_5_bit,
//                                 ns::some_bitmask_flag_bits::flag_6_bit,
//                                 ns::some_bitmask_flag_bits::flag_7_bit);
// using some_bitmask_flags = deaddev::bitmask<ns::some_bitmask_flag_bits>;
//

int main() {
  ns::some_bitmask_flags flags;
  flags |= ns::some_bitmask_flag_bits::flag_1_bit;
  flags = ~flags;
  flags = flags ^ (ns::some_bitmask_flag_bits::flag_5_bit |
                   ns::some_bitmask_flag_bits::flag_4_bit |
                   ns::some_bitmask_flag_bits::flag_1_bit);
  flags.set(ns::some_bitmask_flag_bits::flag_5_bit)
      .remove(ns::some_bitmask_flag_bits::flag_2_bit);

  assert(flags.is_set(ns::some_bitmask_flag_bits::flag_1_bit));

  assert(flags.is_set(ns::some_bitmask_flag_bits::flag_1_bit |
                      ns::some_bitmask_flag_bits::flag_3_bit));

  assert(!flags.is_set(ns::some_bitmask_flag_bits::flag_2_bit));

  assert(!flags.is_set(ns::some_bitmask_flag_bits::flag_2_bit |
                       ns::some_bitmask_flag_bits::flag_4_bit));

  assert(!flags.is_set(ns::some_bitmask_flag_bits::flag_0_bit |
                       ns::some_bitmask_flag_bits::flag_2_bit));
  return 0;
}