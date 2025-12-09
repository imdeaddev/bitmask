# Bitmask library

![C++14](https://img.shields.io/badge/C%2B%2B-14-green)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Build and run tests using CMake](https://github.com/imdeaddev/bitmask/actions/workflows/ctest.yml/badge.svg)](https://github.com/imdeaddev/bitmask/actions/workflows/ctest.yml)

Bitmask is a library for using C++ scoped and unscoped enumerations as a bitmask (flags).
I took idea from Vulkan-Hpp and designed deaddev::bitmask with the same purpose as a vk::Flags

## Installation

Just download and include [deaddev/bitmask.hpp](include/deaddev/bitmask.hpp)

## Usage

```cpp
#include <cassert>
#include <deaddev/bitmask.hpp>

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

int main() {
  some_bitmask_flags flags;
  flags |= some_bitmask_flag_bits::flag_1_bit;
  flags = ~flags;
  flags = flags ^ (some_bitmask_flag_bits::flag_5_bit |
                   some_bitmask_flag_bits::flag_4_bit |
                   some_bitmask_flag_bits::flag_1_bit);
  flags.set(some_bitmask_flag_bits::flag_5_bit)
      .remove(some_bitmask_flag_bits::flag_2_bit);

  assert(flags.is_set(some_bitmask_flag_bits::flag_1_bit));

  assert(flags.is_set(some_bitmask_flag_bits::flag_1_bit |
                      some_bitmask_flag_bits::flag_3_bit));

  assert(!flags.is_set(some_bitmask_flag_bits::flag_2_bit));

  assert(!flags.is_set(some_bitmask_flag_bits::flag_2_bit |
                       some_bitmask_flag_bits::flag_4_bit));

  assert(!flags.is_set(some_bitmask_flag_bits::flag_0_bit |
                       some_bitmask_flag_bits::flag_2_bit));
  return 0;
}
```

## License

[MIT License](LICENSE)

## Libraries used

[Catch2](https://github.com/catchorg/Catch2) (testing only) BSL-1.0 License
