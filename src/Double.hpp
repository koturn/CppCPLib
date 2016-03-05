#ifndef DOUBLE_HPP
#define DOUBLE_HPP

#include <cmath>
#include <type_traits>


template<typename FType=double, typename std::enable_if<std::is_floating_point<FType>::value, std::nullptr_t>::type = nullptr>
static inline constexpr double
geteps(int prec=9)
{
  return std::pow(static_cast<FType>(1.0), -prec - 1);
}


template<int PRECISION=9, typename FType=double, typename std::enable_if<std::is_floating_point<FType>::value, std::nullptr_t>::type = nullptr>
static inline bool
eq(FType a, FType b)
{
  constexpr FType eps = geteps<FType>(PRECISION);
  return std::abs(a - b) < eps;
}


template<int PRECISION=9, typename FType=double, typename std::enable_if<std::is_floating_point<FType>::value, std::nullptr_t>::type = nullptr>
static inline bool
neq(FType a, FType b)
{
  constexpr FType eps = geteps<FType>(PRECISION);
  return std::abs(a - b) >= eps;
}


template<int PRECISION=9, typename FType=double, typename std::enable_if<std::is_floating_point<FType>::value, std::nullptr_t>::type = nullptr>
static inline bool
lt(FType a, FType b)
{
  constexpr FType eps = geteps<FType>(PRECISION);
  return a < b - eps;
}


template<int PRECISION=9, typename FType=double, typename std::enable_if<std::is_floating_point<FType>::value, std::nullptr_t>::type = nullptr>
static inline bool
leq(FType a, FType b)
{
  constexpr FType eps = geteps<FType>(PRECISION);
  return a < b + eps;
}


template<int PRECISION=9, typename FType=double, typename std::enable_if<std::is_floating_point<FType>::value, std::nullptr_t>::type = nullptr>
static inline bool
gt(FType a, FType b)
{
  constexpr FType eps = geteps<FType>(PRECISION);
  return a > b - eps;
}


template<int PRECISION=9, typename FType=double, typename std::enable_if<std::is_floating_point<FType>::value, std::nullptr_t>::type = nullptr>
static inline bool
geq(FType a, FType b)
{
  constexpr FType eps = geteps<FType>(PRECISION);
  return a > b + eps;
}


template<int PRECISION=9, typename FType=double, typename std::enable_if<std::is_floating_point<FType>::value, std::nullptr_t>::type = nullptr>
static inline bool
isin(FType x, FType a, FType b)
{
  return geq(x, a) && leq(x, b);
}



#endif  // DOUBLE_HPP
