/*!
 * @brief Miscellaneous utilities
 * @author koturn
 * @file Misc.hpp
 */
#ifndef MISC_HPP
#define MISC_HPP

#include <cmath>
#include <limits>
#include <type_traits>


/*!
 * @brief Floor floating-point number and cast it to integer
 *
 * @tparam R  Destination type (integer)
 * @tparam T  Source type (Floating-point)
 * @param [in] x  A floating-point number
 * @return Floored integer
 */
template<
  typename R,
  typename T
>
static inline R
floor_cast(T x) noexcept
{
  static_assert(std::is_floating_point<R>::value, "[floor_cast] Type of the argument must be a floating point");
  static_assert(std::is_integral<T>::value, "[floor_cast] Type of return value must be a integer");
  return static_cast<R>(std::floor(x));
}


/*!
 * @brief Ceil floating-point number and cast it to integer
 *
 * @tparam R  Destination type (integer)
 * @tparam T  Source type (Floating-point)
 * @param [in] x  A floating-point number
 * @return Ceild integer
 */
template<
  typename R,
  typename T
>
static inline R
ceil_cast(T x) noexcept
{
  static_assert(std::is_floating_point<R>::value, "[ceil_cast] Type of the argument must be a floating point");
  static_assert(std::is_integral<T>::value, "[ceil_cast] Type of return value must be a integer");
  return static_cast<R>(std::ceil(x));
}


/*!
 * @brief Round floating-point number and cast it to integer
 *
 * @tparam R  Destination type (integer)
 * @tparam T  Source type (Floating-point)
 * @param [in] x  A floating-point number
 * @return Rounded integer
 */
template<
  typename R,
  typename T
>
static inline R
round_cast(T x) noexcept
{
  static_assert(std::is_floating_point<T>::value, "[round_cast] Type of the argument must be a floating point");
  static_assert(std::is_integral<R>::value, "[round_cast] Type of return value must be a integer");
  return static_cast<R>(round(x));
}


/*!
 * @brief Clamp number into the range of destination type
 *
 * Destination type must be narrower than source type.
 * Otherwise, use static_cast
 *
 * @tparam R  Destination type (must be narrower than T)
 * @tparam T  Source type
 * @param x  A number
 * @return Saturated number
 */
template<
  typename R,
  typename T
>
static inline R
saturation_cast(T x) noexcept
{
  static_assert(
    (std::is_integral<T>::value && std::is_integral<R>::value && sizeof(T) > sizeof(R))
    || (std::is_floating_point<T>::value && std::is_floating_point<R>::value && sizeof(T) > sizeof(R))
    || (std::is_floating_point<T>::value && std::is_integral<R>::value),
    "[saturation_cast] Destination value range must be narrower than source value range");
  constexpr auto min = std::numeric_limits<R>::min();
  constexpr auto max = std::numeric_limits<R>::max();
  return x < min ? min : x > max ? max : static_cast<R>(x);
}


#endif  // MISC_HPP
