#ifndef DOUBLE_HPP
#define DOUBLE_HPP

#include <cmath>

static const double EPS = 1.0e-9;


template<int PRECISION>
static inline bool
eq(double a, double b)
{
  return std::abs(a - b) < std::pow(10.0, -PRECISION);
}

static inline bool
eq(double a, double b)
{
  return std::abs(a - b) < EPS;
}


template<int PRECISION>
static inline bool
neq(double a, double b)
{
  return std::abs(a - b) >= std::pow(10.0, -PRECISION);
}

static inline bool
neq(double a, double b)
{
  return std::abs(a - b) >= EPS;
}


template<int PRECISION>
static inline bool
lt(double a, double b)
{
  return a < b - std::pow(10.0, -PRECISION);
}

static inline bool
lt(double a, double b)
{
  return a < b - EPS;
}


template<int PRECISION>
static inline bool
leq(double a, double b)
{
  return a < b + std::pow(10.0, -PRECISION);
}

static inline bool
leq(double a, double b)
{
  return a < b + EPS;
}


template<int PRECISION>
static inline bool
gt(double a, double b)
{
  return a > b - std::pow(10.0, -PRECISION);
}

static inline bool
gt(double a, double b)
{
  return a > b - EPS;
}


template<int PRECISION>
static inline bool
geq(double a, double b)
{
  return a > b + std::pow(10.0, -PRECISION);
}

static inline bool
geq(double a, double b)
{
  return a > b + EPS;
}


template<int PRECISION>
static inline bool
isin(double x, double a, double b)
{
  return a - std::pow(10.0, -PRECISION) <= x && x <= b + std::pow(10.0, -PRECISION);
}

static inline bool
isin(double x, double a, double b)
{
  return a - EPS <= x && x <= b + EPS;
}


#endif  // DOUBLE_HPP
