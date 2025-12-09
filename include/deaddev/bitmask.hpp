///////////////////////////////////////////////////////////////////////////////////////
//                                                                                   //
//  MIT License                                                                      //
//                                                                                   //
//  Copyright (c) 2025 DeadDev                                                       //
//                                                                                   //
//  Permission is hereby granted, free of charge, to any person obtaining a copy     //
//  of this software and associated documentation files (the "Software"), to deal    //
//  in the Software without restriction, including without limitation the rights     //
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell        //
//  copies of the Software, and to permit persons to whom the Software is            //
//  furnished to do so, subject to the following conditions:                         //
//                                                                                   //
//  The above copyright notice and this permission notice shall be included in all   //
//  copies or substantial portions of the Software.                                  //
//                                                                                   //
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR       //
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,         //
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE      //
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER           //
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,    //
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE    //
//  SOFTWARE.                                                                        //
//                                                                                   //
///////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Wrapper for C++ enums to use them as a bit masks
 * @details Wrapper and utilities that allows you to safely use C++ enumerations as a bit
 * masks
 * @author DeadDev https://github.com/imdeaddev
 * @date 2025-12-09
 * @copyright DeadDev (C) 2025, MIT license
 */

/// include guard
#ifndef DEADDEV_BITMASK_HPP
#define DEADDEV_BITMASK_HPP
#pragma once
#include <climits>
#include <type_traits>
#include <cstddef>

#ifndef DEADDEV_CONSTEVAL
#if __cplusplus >= 202002L
#define DEADDEV_CONSTEVAL consteval
#else
#define DEADDEV_CONSTEVAL constexpr
#endif
#endif

#ifndef DEADDEV_NODISCARD
#if defined(__has_cpp_attribute) && __has_cpp_attribute(nodiscard)
#define DEADDEV_NODISCARD [[nodiscard]]
#else
#define DEADDEV_NODISCARD
#endif
#endif

/**
 * @brief it's me :)
 */
namespace deaddev {

/**
 * @brief some utility structures and functions
 */
namespace details {

#ifdef __cpp_lib_is_scoped_enum
/**
 * @brief because older c++ standard don't support std::is_scoped_enum_v
 *
 * @tparam T enum type
 */
template <typename T> constexpr bool is_scoped_enum_v = ::std::is_scoped_enum_v<T>;
#elif defined(__has_builtin) && __has_builtin(__is_scoped_enum)

/**
 * @brief c++14 compatible std::is_scoped_enum
 *
 * @tparam T enum type
 */
template <typename T> constexpr bool is_scoped_enum_v = __is_scoped_enum(T);
#else
/**
 * @brief c++14 compatible std::is_scoped_enum
 *
 * @tparam T enum type
 */
template <typename T>
constexpr bool is_scoped_enum_v =
    ::std::is_enum<T>::value &&
    !::std::is_convertible<T, typename ::std::underlying_type<T>::type>::value;
#endif

/**
 * @brief default value
 * @details bitmasks are disabled by default
 */
struct empty_bitmask_traits {
  /// for sfinae type checks
  static constexpr bool enable = false;
  /// in case if `DEADDEV_ENABLE_BITMASKS_FOR_SCOPED_ENUMS` is
  /// defined
  static constexpr size_t all_flags = 0xFFFFFFFFFFFFFFFF;
};

/**
 *
 * @brief Flag combination routine
 * @tparam T flag type
 * @param result
 * @param value
 */
template <typename T>
DEADDEV_CONSTEVAL auto combine_flags(::std::underlying_type_t<T> &result, T value)
    -> void {
  result |= static_cast<::std::underlying_type_t<T>>(value);
}

#if __cplusplus < 201703L
/**
 * @brief sentinel recursion overload for all flags calculation
 *
 * @tparam T enum type
 * @param val value
 * @return T value
 */
template <typename T>
DEADDEV_CONSTEVAL auto calculate_all_flags_cpp_before_17(T val) -> T {
  return static_cast<T>(val);
}
/**
 * @brief C++14 implementation for calculate_all_flags
 *
 * @tparam T enum type
 * @tparam Args enum values
 * @param arg first enum value
 * @param args rest of values
 * @return T combined valuees
 */
template <typename T, typename... Args>
DEADDEV_CONSTEVAL auto calculate_all_flags_cpp_before_17(T arg, Args... args) -> T {
  return static_cast<T>(static_cast<typename ::std::underlying_type<T>::type>(arg) |
                        static_cast<typename ::std::underlying_type<T>::type>(
                            calculate_all_flags_cpp_before_17(args...)));
}
#else
template <typename T, typename... Args>
DEADDEV_CONSTEVAL auto calculate_all_flags_cpp_17(Args... args) -> T {
  ::std::underlying_type_t<T> result{0};
  (::deaddev::details::combine_flags(result, args), ...);
  return static_cast<T>(result);
}
#endif

/**
 * @brief Combines flags
 * @details Uses folding expressions to easily combine all flags using binary or operator
 * @tparam T enum type
 * @tparam Args enum types or integers
 * @param args all flags list
 * @return T all flags combination
 */
template <typename T, typename... Args>
DEADDEV_CONSTEVAL auto calculate_all_flags(Args... args) -> T {
#if __cplusplus < 201703L
  return calculate_all_flags_cpp_before_17<T>(args...);
#else
  return calculate_all_flags_cpp_17<T>(args...);
#endif
}
/**
 * @brief returns all flags
 * @details there's no all flags specified then library will use (T)-1
 * @tparam T bitmask type
 * @return T value with all bits are set to 1
 */
template <typename T> DEADDEV_CONSTEVAL auto calculate_all_flags() -> T {
  return static_cast<T>((1 << (sizeof(T) * CHAR_BIT)) - 1);
}

/**
 * @brief used for deaddev::details::is_bitmask_v<T> and
 * deaddev::details::bitmask_all_flags_v<T>
 * @details actual implementation of bitmask type trait
 * @tparam T enum type
 * @tparam AllFlags all flags combination
 */
template <typename T, T AllFlags> struct bitmask_traits {
  /// for sfinae checks
  static constexpr bool enable = true;
  /// for better negation
  static constexpr T all_flags = AllFlags;
};

/**
 * @brief noop
 * @details This function does nothing but it's used to get bitmask traits type using
 * argument-dependent lookup (ADL)
 * @param ... placeholder for ADL
 * @return ::deaddev::details::empty_bitmask_traits default traits
 */
auto adl_bitmask_operations_check(...) -> ::deaddev::details::empty_bitmask_traits;

/**
 * @brief trait implementation
 * @details wrapper for cases when we can't use ADL (e.g. for external libraries)
 * @tparam T bitmask type
 */
template <typename T>
struct bitmask_operations_check_traits
    : decltype(adl_bitmask_operations_check(::std::declval<T &>())){};

/**
 * @brief Is T a bitmask type
 * @details Check if the bitmask operations are enabled for T
 * @tparam T enum type
 * @tparam typename SFINAE
 */
template <typename T,
          typename = typename ::std::enable_if<::std::is_enum<T>::value>::type>
constexpr bool is_bitmask_v =
#ifdef DEADDEV_ENABLE_BITMASKS_FOR_SCOPED_ENUMS
    ::deaddev::details::is_scoped_enum_v<T> ||
#endif
    ::deaddev::details::bitmask_operations_check_traits<T>::enable;

/**
 * @brief All flags combined
 * @details All flags combination for better negation
 * @tparam T enum type
 * @tparam typename SFINAE
 */
template <typename T,
          typename = typename ::std::enable_if<::std::is_enum<T>::value>::type>
constexpr T bitmask_all_flags_v =
    ::deaddev::details::bitmask_operations_check_traits<T>::all_flags;

} // namespace details

/**
 * @brief Bitmask class type
 * @details Wrapper for enums to use their values as bit flags/masks
 * It e
 * @tparam T Enum type
 * @tparam typename
 */
template <typename T,
          typename = typename ::std::enable_if<::deaddev::details::is_bitmask_v<T>>::type>
class bitmask {

public:
  /// underlying type of enum
  using mask_type = ::std::underlying_type_t<T>;
  /// original enum
  using enum_type = T;
  /// default constructor
  constexpr bitmask() noexcept = default;
  /// copy constructor
  constexpr bitmask(const bitmask &) noexcept = default;
  /// move constructor
  constexpr bitmask(bitmask &&) noexcept = default;
  /// copy assignment operator
  constexpr bitmask &operator=(const bitmask &) noexcept = default;
  /// move assignment operator
  constexpr bitmask &operator=(bitmask &&) noexcept = default;
  /**
   * @brief mask from enum constructor
   * @details implicit cast from enum_type
   */
  constexpr bitmask(enum_type value) noexcept : mask_(static_cast<mask_type>(value)) {}
  /**
   * @brief mask from integral type assignment operator
   * @details explicit cast from mask_type
   */
  explicit constexpr bitmask(mask_type value) noexcept : mask_(value) {}

  /**
   * @brief explicit cast to original enum type
   * @warning returned value is not a valid enum member in most cases!
   * @return enum_type bitmask value as enum type
   */
  DEADDEV_NODISCARD explicit constexpr operator enum_type() const noexcept {
    return static_cast<enum_type>(mask_);
  }
  /**
   * @brief explicit cast to integral type
   * @warning returned value is not a valid enum member in most cases!
   * @return mask_type bitmask value as integral type
   */
  DEADDEV_NODISCARD explicit constexpr operator mask_type() const noexcept {
    return mask_;
  }

  /**
   * @brief returns bitmask with all flags set
   * @return bitmask all flags combination
   */
  DEADDEV_NODISCARD DEADDEV_CONSTEVAL static bitmask all_flags() noexcept {
    return bitmask(::deaddev::details::bitmask_all_flags_v<enum_type>);
  }

  /// comparison operator
  DEADDEV_NODISCARD constexpr bool operator==(mask_type mask) const noexcept {
    return mask_ == mask;
  }
  /// comparison operator
  DEADDEV_NODISCARD constexpr bool operator!=(mask_type mask) const noexcept {
    return mask_ != mask;
  }
  /// comparison operator
  DEADDEV_NODISCARD constexpr bool operator<=(mask_type mask) const noexcept {
    return mask_ <= mask;
  }
  /// comparison operator
  DEADDEV_NODISCARD constexpr bool operator>=(mask_type mask) const noexcept {
    return mask_ >= mask;
  }
  /// comparison operator
  DEADDEV_NODISCARD constexpr bool operator<(mask_type mask) const noexcept {
    return mask_ < mask;
  }
  /// comparison operator
  DEADDEV_NODISCARD constexpr bool operator>(mask_type mask) const noexcept {
    return mask_ > mask;
  }
  /// comparison operator
  DEADDEV_NODISCARD constexpr bool operator==(bitmask other) const noexcept {
    return mask_ == other.mask_;
  }
  /// comparison operator
  DEADDEV_NODISCARD constexpr bool operator!=(bitmask other) const noexcept {
    return mask_ != other.mask_;
  }
  /// comparison operator
  DEADDEV_NODISCARD constexpr bool operator<=(bitmask other) const noexcept {
    return mask_ <= other.mask_;
  }
  /// comparison operator
  DEADDEV_NODISCARD constexpr bool operator>=(bitmask other) const noexcept {
    return mask_ >= other.mask_;
  }
  /// comparison operator
  DEADDEV_NODISCARD constexpr bool operator<(bitmask other) const noexcept {
    return mask_ < other.mask_;
  }
  /// comparison operator
  DEADDEV_NODISCARD constexpr bool operator>(bitmask other) const noexcept {
    return mask_ > other.mask_;
  }

  /// comparison operator
  DEADDEV_NODISCARD constexpr bitmask operator~() const noexcept {
    return bitmask(static_cast<mask_type>(mask_ ^ all_flags().mask_));
  }

  /// comparison operator
  DEADDEV_NODISCARD constexpr bitmask operator^(bitmask other) const noexcept {
    return bitmask(mask_ ^ other.mask_);
  }

  /// comparison operator
  DEADDEV_NODISCARD constexpr bitmask operator|(bitmask other) const noexcept {
    return bitmask(mask_ | other.mask_);
  }

  /// comparison operator
  DEADDEV_NODISCARD constexpr bitmask operator&(bitmask other) const noexcept {
    return bitmask(mask_ & other.mask_);
  }

  /// comparison operator
  constexpr bitmask &operator^=(bitmask other) noexcept {
    mask_ ^= other.mask_;
    return *this;
  }

  /// comparison operator
  constexpr bitmask &operator|=(bitmask other) noexcept {
    mask_ |= other.mask_;
    return *this;
  }

  /// comparison operator
  constexpr bitmask &operator&=(bitmask other) noexcept {
    mask_ &= other.mask_;
    return *this;
  }

  /// comparison operator
  DEADDEV_NODISCARD constexpr bitmask operator^(enum_type other) const noexcept {
    return bitmask(mask_ ^ static_cast<mask_type>(other));
  }

  /// comparison operator
  DEADDEV_NODISCARD constexpr bitmask operator|(enum_type other) const noexcept {
    return bitmask(mask_ | static_cast<mask_type>(other));
  }

  /// comparison operator
  DEADDEV_NODISCARD constexpr bitmask operator&(enum_type other) const noexcept {
    return bitmask(mask_ & static_cast<mask_type>(other));
  }

  /// comparison operator
  constexpr bitmask &operator^=(enum_type other) noexcept {
    mask_ ^= static_cast<mask_type>(other);
    return *this;
  }

  /// comparison operator
  constexpr bitmask &operator|=(enum_type other) noexcept {
    mask_ |= static_cast<mask_type>(other);
    return *this;
  }

  /// comparison operator
  constexpr bitmask &operator&=(enum_type other) noexcept {
    mask_ &= static_cast<mask_type>(other);
    return *this;
  }

  /**
   * @brief Checks if an enum value is in the mask
   * @details Performs comparison between the flag and bitwise and between this value and
   * flag
   * @param flag enum value
   * @return true flag is in the mask
   * @return false flag is not in the mask
   */
  DEADDEV_NODISCARD constexpr bool is_set(enum_type flag) const noexcept {
    return (mask_ & static_cast<mask_type>(flag)) == static_cast<mask_type>(flag);
  }

  /**
   * @brief Checks if all flags from other bit mask is set in this bit mask
   * @details Performs comparison between the other value and bitwise and between this
   * value and the other value
   * @param other bit mask
   * @return true all flags is in the mask
   * @return false some or all flags is not in the mask
   */
  DEADDEV_NODISCARD constexpr bool is_set(bitmask other) const noexcept {
    return (mask_ & other.mask_) == other.mask_;
  }

  /**
   * @brief Combine bit mask with an enum value
   * @details Performs call to bitwise "or" assignment operator
   * @param flag enum value
   * @return bitmask& call chaining
   */
  constexpr bitmask &set(enum_type flag) noexcept { return *this |= flag; }

  /**
   * @brief Combine bit mask with an enum value
   * @details Performs call to bitwise "or" assignment operator
   * @param other bit mask
   * @return bitmask& call chaining
   */
  constexpr bitmask &set(bitmask other) noexcept { return *this |= other.mask_; }

  /**
   * @brief Combine bit mask with an enum value
   * @details Performs call to bitwise "or" assignment operator
   * @param flag enum value
   * @return bitmask& call chaining
   */
  constexpr bitmask &remove(enum_type flag) noexcept { return *this ^= flag; }

  /**
   * @brief Combine bit mask with an enum value
   * @details Performs call to bitwise "or" assignment operator
   * @param other bit mask
   * @return bitmask& call chaining
   */
  constexpr bitmask &remove(bitmask other) noexcept { return *this ^= other.mask_; }

private:
  /// bit mask value
  mask_type mask_{};
};

} // namespace deaddev

// Out of namespace because of ADL

template <typename T, typename = ::std::enable_if<::deaddev::details::is_bitmask_v<T>>>
DEADDEV_NODISCARD constexpr bool operator==(T left, deaddev::bitmask<T> right) noexcept {
  return right == left; // reversed order
}
template <typename T, typename = ::std::enable_if<::deaddev::details::is_bitmask_v<T>>>
DEADDEV_NODISCARD constexpr bool operator!=(T left, deaddev::bitmask<T> right) noexcept {
  return right != left; // reversed order
}
template <typename T, typename = ::std::enable_if<::deaddev::details::is_bitmask_v<T>>>
DEADDEV_NODISCARD constexpr bool operator>=(T left, deaddev::bitmask<T> right) noexcept {
  return right <= left; // reversed order
}
template <typename T, typename = ::std::enable_if<::deaddev::details::is_bitmask_v<T>>>
DEADDEV_NODISCARD constexpr bool operator<=(T left, deaddev::bitmask<T> right) noexcept {
  return right >= left; // reversed order
}
template <typename T, typename = ::std::enable_if<::deaddev::details::is_bitmask_v<T>>>
DEADDEV_NODISCARD constexpr bool operator<(T left, deaddev::bitmask<T> right) noexcept {
  return right > left; // reversed order
}
template <typename T, typename = ::std::enable_if<::deaddev::details::is_bitmask_v<T>>>
DEADDEV_NODISCARD constexpr bool operator>(T left, deaddev::bitmask<T> right) noexcept {
  return right < left; // reversed order
}
template <typename T, typename = ::std::enable_if<::deaddev::details::is_bitmask_v<T>>>
DEADDEV_NODISCARD constexpr bool operator==(typename std::underlying_type<T>::type left,
                                            deaddev::bitmask<T> right) noexcept {
  return right == left; // reversed order
}
template <typename T, typename = ::std::enable_if<::deaddev::details::is_bitmask_v<T>>>
DEADDEV_NODISCARD constexpr bool operator!=(typename std::underlying_type<T>::type left,
                                            deaddev::bitmask<T> right) noexcept {
  return right != left; // reversed order
}
template <typename T, typename = ::std::enable_if<::deaddev::details::is_bitmask_v<T>>>
DEADDEV_NODISCARD constexpr bool operator>=(typename std::underlying_type<T>::type left,
                                            deaddev::bitmask<T> right) noexcept {
  return right <= left; // reversed order
}
template <typename T, typename = ::std::enable_if<::deaddev::details::is_bitmask_v<T>>>
DEADDEV_NODISCARD constexpr bool operator<=(typename std::underlying_type<T>::type left,
                                            deaddev::bitmask<T> right) noexcept {
  return right >= left; // reversed order
}
template <typename T, typename = ::std::enable_if<::deaddev::details::is_bitmask_v<T>>>
DEADDEV_NODISCARD constexpr bool operator<(typename std::underlying_type<T>::type left,
                                           deaddev::bitmask<T> right) noexcept {
  return right > left; // reversed order
}
template <typename T, typename = ::std::enable_if<::deaddev::details::is_bitmask_v<T>>>
DEADDEV_NODISCARD constexpr bool operator>(typename std::underlying_type<T>::type left,
                                           deaddev::bitmask<T> right) noexcept {
  return right < left; // reversed order
}

template <typename T, typename = ::std::enable_if<::deaddev::details::is_bitmask_v<T>>>
DEADDEV_NODISCARD constexpr deaddev::bitmask<T> operator~(T flag) noexcept {
  return ~deaddev::bitmask<T>(flag);
}

template <typename T, typename = ::std::enable_if<::deaddev::details::is_bitmask_v<T>>>
DEADDEV_NODISCARD constexpr deaddev::bitmask<T> operator^(T left, T right) noexcept {
  return deaddev::bitmask<T>(left) ^ right;
}

template <typename T, typename = ::std::enable_if<::deaddev::details::is_bitmask_v<T>>>
DEADDEV_NODISCARD constexpr deaddev::bitmask<T> operator|(T left, T right) noexcept {
  return deaddev::bitmask<T>(left) | right;
}

template <typename T, typename = ::std::enable_if<::deaddev::details::is_bitmask_v<T>>>
DEADDEV_NODISCARD constexpr deaddev::bitmask<T> operator&(T left, T right) noexcept {
  return deaddev::bitmask<T>(left) & right;
}

/**
 * @brief Enable bitmask operations for enum
 * @details defines `auto adl_bitmask_operations_check(T) ->
 * ::deaddev::details::bitmask_traits<T, ...>` function for ADL-based type checks
 * @param T enum type
 * @param ... optional enum values list for all flags calculation
 */
#define DEADDEV_ENABLE_BITMASK(T, ...)                                                   \
  auto adl_bitmask_operations_check(T &) -> ::deaddev::details::bitmask_traits<          \
      T, ::deaddev::details::calculate_all_flags<T>(__VA_ARGS__)>;

/**
 * @brief Enable bitmask operations for enum
 * @details defines template specialization for
 * `struct ::deaddev::details::bitmask_operations_check_traits<T>`
 * for ADL-independent type checks
 * @param T enum type
 * @param ... optional enum values list for all flags calculation
 */
#define DEADDEV_ENABLE_BITMASK_EXTERNAL(T, ...)                                          \
  template <>                                                                            \
  struct ::deaddev::details::bitmask_operations_check_traits<T>                          \
      : ::deaddev::details::bitmask_traits<                                              \
            T, ::deaddev::details::calculate_all_flags<T>(__VA_ARGS__)> {};

#endif // DEADDEV_BITMASK_HPP
