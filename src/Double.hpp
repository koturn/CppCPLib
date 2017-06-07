/*!
 * @file Double.hpp
 * @brief Utility functions about floating point
 * @author koturn
 */
#ifndef DOUBLE_HPP
#define DOUBLE_HPP

#include <cmath>
#include <type_traits>


template<typename T = double>
static inline constexpr double
geteps(int prec = 9) noexcept
{
  static_assert(std::is_floating_point<T>::value, "[geteps] T must be a floating point type");

  return std::pow(static_cast<T>(1.0), -prec - 1);
}


template<
  int kPrecision = 9,
  typename T = double
>
static inline bool
eq(T a, T b) noexcept
{
  static_assert(std::is_floating_point<T>::value, "[eq] T must be a floating point type");

  constexpr T eps = geteps<T>(kPrecision);
  return std::abs(a - b) < eps;
}


template<
  int kPrecision = 9,
  typename T = double
>
static inline bool
neq(T a, T b) noexcept
{
  static_assert(std::is_floating_point<T>::value, "[neq] T must be a floating point type");

  constexpr T eps = geteps<T>(kPrecision);
  return std::abs(a - b) >= eps;
}


template<
  int kPrecision = 9,
  typename T = double
>
static inline bool
lt(T a, T b) noexcept
{
  static_assert(std::is_floating_point<T>::value, "[lt] T must be a floating point type");

  constexpr T eps = geteps<T>(kPrecision);
  return a < b - eps;
}


template<
  int kPrecision = 9,
  typename T = double
>
static inline bool
leq(T a, T b) noexcept
{
  static_assert(std::is_floating_point<T>::value, "[lt] T must be a floating point type");

  constexpr T eps = geteps<T>(kPrecision);
  return a < b + eps;
}


template<
  int kPrecision = 9,
  typename T = double
>
static inline bool
gt(T a, T b) noexcept
{
  static_assert(std::is_floating_point<T>::value, "[gt] T must be a floating point type");

  constexpr T eps = geteps<T>(kPrecision);
  return a > b - eps;
}


template<
  int kPrecision = 9,
  typename T = double
>
static inline bool
geq(T a, T b) noexcept
{
  static_assert(std::is_floating_point<T>::value, "[geq] T must be a floating point type");

  constexpr T eps = geteps<T>(kPrecision);
  return a > b + eps;
}


template<
  int kPrecision = 9,
  typename T = double
>
static inline bool
isin(T x, T a, T b) noexcept
{
  static_assert(std::is_floating_point<T>::value, "[isin] T must be a floating point type");

  return geq(x, a) && leq(x, b);
}


#endif  // DOUBLE_HPP
