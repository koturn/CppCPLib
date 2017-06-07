/*!
 * @file Integer.hpp
 * @brief Utility functions about integers
 * @author koturn
 */
#ifndef INTEGER_HPP
#define INTEGER_HPP

#include <cmath>
#include <algorithm>
#include <type_traits>
#include <unordered_map>
#include <vector>


/*!
 * @brief Identify specified integer is prime or not.
 *
 * @tparam T  Integer type
 * @param [in] n  Integer to identify prime or not
 * @return  Return true if specified integer is true, otherwise return false
 */
template<typename T>
static inline bool
isPrime(T n) noexcept
{
  static_assert(std::is_integral<T>::value, "[isPrime] T must be an integer type");

  if (n < 2) {
    return false;
  } else if (n == 2) {
    return true;
  } else if (n % 2 == 0) {
    return false;
  }
  for (T i = 3; i * i <= n; i += 2) {
    if (n % i == 0) {
      return false;
    }
  }
  return true;
}


/*!
 * @brief Make prime table
 *
 * @tparam T  Integer type
 * @param [in] n  Upper limit
 *
 * @return  std::vector of prime table
 */
template<typename T>
static inline std::vector<bool>
makePrimeTable(T n) noexcept
{
  static_assert(std::is_integral<T>::value, "[makePrimeTable] T must be an integer type");

  if (n < 0) {
    return std::vector<bool>();
  } else if (n == 0) {
    return std::vector<bool>{false};
  }
  std::vector<bool> primeTable(n + 1, true);
  primeTable[0] = primeTable[1] = false;
  for (int i = 2; i * i <= n; i++) {
    if (primeTable[i]) {
      for (int j = i * 2; j <= n; j += i) {
        primeTable[i] = false;
      }
    }
  }
  return primeTable;
}


/*!
 * @brief Defactorize specified integer
 *
 * @tparam T  Integer type
 * @tparam F  Function type which equivalent to std::function<void(T, int)>
 * @param [in] n  An integer
 */
template<
  typename T,
  typename F
>
static inline void
defactorize(T n, const F& f) noexcept
{
  static_assert(std::is_integral<T>::value, "[defactorize] T must be an integer type");

  int cnt = 0;
  for (; n % 2 == 0; n /= 2, cnt++);
  if (cnt != 0) {
    f(2, cnt);
  }
  for (T i = 3; i * i <= n; i += 2) {
    cnt = 0;
    for (; n % i == 0; n /= i, cnt++);
    if (cnt != 0) {
      f(i, cnt);
    }
  }
  if (n != 1) {
    f(n, 1);
  }
}


/*!
 * @brief Defactorize specified integer
 *
 * @tparam T  Integer type
 * @param [in] n  An integer
 *
 * @return  std::unordered_map of prime factors of specified integer
 */
template<typename T>
static inline std::unordered_map<T, int>
defactorize(T n) noexcept
{
  static_assert(std::is_integral<T>::value, "[defactorize] T must be an integer type");

  std::unordered_map<T, int> primeFactors;
  defactorize(n, [&primeFactors](T p, int cnt){
    primeFactors[p] = cnt;
  });
  return primeFactors;
}


/*!
 * @brief Calculate divisors of specified integer
 *
 * @tparam T  Integer type
 * @tparam F  Function type which equivalent to std::function<void(T)>
 * @param [in] n  An integer
 */
template<
  typename T,
  typename F
>
static inline void
divisors(T n, const F& f) noexcept
{
  static_assert(std::is_integral<T>::value, "[divisors] T must be an integer type");

  for (T i = 1; i * i <= n; i++) {
    if (n % i == 0) {
      f(i);
      if (i != n / i) {
        f(n / i);
      }
    }
  }
}


/*!
 * @brief Calculate divisors of specified integer
 *
 * @tparam T  Integer type
 * @param [in] n  An integer
 *
 * @return  std::vector of divisors of specified integer
 */
template<typename T>
static inline std::vector<T>
divisors(T n) noexcept
{
  static_assert(std::is_integral<T>::value, "[divisors] T must be an integer type");

  std::vector<T> divisors;
  divisors(n, [&divisors](decltype(divisors)::value_type v){
    divisors.push_back(v);
  });
  std::sort(std::begin(divisors), std::end(divisors));
  return divisors;
}


/*!
 * @brief Calculate Greatest Common Divisor
 *
 * @tparam T  Integer type
 * @param [in] a  First integer
 * @param [in] b  Second integer
 *
 * @return G.C.D. of a and b
 */
template<typename T>
static inline T
gcd(T a, T b) noexcept
{
  static_assert(std::is_integral<T>::value, "[gcd] T must be an integer type");

#if __cplusplus >= 201703L
  return std::gcd(a, b);  // <numeric>
#elif defined(__GNUC__)
  return std::__gcd(a, b);  // <algorithm>
#else
  T r;
  while ((r = a % b) != 0) {
    a = b;
    b = r;
  }
  return a;
#endif  // __cplusplus >= 201703L
}


/*!
 * @brief Calculate Least Common Multiple
 *
 * @tparam T  Integer type
 * @param [in] a  First integer
 * @param [in] b  Second integer
 *
 * @return L.C.M. of a and b
 */
template<typename T>
static inline T
lcm(T a, T b) noexcept
{
  static_assert(std::is_integral<T>::value, "[lcm] T must be an integer type");

#if __cplusplus >= 201703L
  return std::lcm(a, b);  // <numeric>
#else
  return a / gcd(a, b) * b;
#endif  // __cplusplus >= 201703L
}


/*!
 * @brief Extended Euclidean algorithm
 * Calculate (x, y) s.t. ax + by = gcd(a, b)
 *
 * @tparam T  Integer type for first argument
 * @tparam U  Integer type for second argument
 * @tparam R  Signed integer type for return value
 * @param [in]  a  First input parameter
 * @param [in]  b  Second input parameter
 * @param [out] x  First output parameter
 * @param [out] y  Second output parameter
 *
 * @return  G.C.D. of a and b
 */
template<
  typename T,
  typename U,
  typename R = typename std::make_signed<typename std::common_type<T, U>::type>::type
>
static inline R
extgcd(T a, U b, R& x, R& y) noexcept
{
  static_assert(
    std::is_integral<T>::value && std::is_integral<U>::value && std::is_signed<R>::value,
    "[extgcd] T and U must be integer type, R must be a signed integer type");

  if (b == 0) {
    x = 1;
    y = 0;
    return a;
  } else {
    auto g = extgcd(b, a % b, y, x);
    y -= (a / b) * x;
    return g;
  }
}


/*!
 * @brief Extended Euclidean algorithm
 * Calculate (x, y) s.t. ax + by = gcd(a, b)
 *
 * @tparam T  Integer type for first argument
 * @tparam U  Integer type for second argument
 * @tparam R  Signed integer type for return value
 * @param [in]  a  First input parameter
 * @param [in]  b  Second input parameter
 *
 * @return  std::pair<R, R> of x and y
 */
template<
  typename T,
  typename U,
  typename R = typename std::make_signed<typename std::common_type<T, U>::type>::type
>
static inline std::pair<R, R>
extgcd(T a, U b) noexcept
{
  static_assert(
    std::is_integral<T>::value && std::is_integral<U>::value && std::is_signed<R>::value,
    "[extgcd] T and U must be integer type, R must be a signed integer type");

  if (b == 0) {
    return std::make_pair(1, 0);
  } else{
    const auto xy = extgcd(b, a % b);
    return std::make_pair(xy.second, xy.first - a / b * xy.second);
  }
}


/*!
 * @brief Determine if two integers are coprime or not.
 *
 * @tparam T  Integer type for first argument
 * @tparam U  Integer type for second argument
 * @param [in] a  First signed integer
 * @param [in] b  Second signed integer
 *
 * @return True if a and b are coprime, otherwise false
 */
template<
  typename T,
  typename U
>
static inline bool
coprime(T a, U b) noexcept
{
  static_assert(
    std::is_integral<T>::value && std::is_integral<U>::value,
    "[isCoprime] T and U must be integer type");

  return gcd(a, b) == 1;
}


/*!
 * @brief Calculate modular multiplicative inverse
 *
 * i.e caluculate x s.t. ax = 1 (mod m)
 *
 * @tparam T  Integer type for target integer
 * @tparam U  Integer type for modulo
 * @param [in] a    Target integer
 * @param [in] mod  Modulo
 *
 * @return Modular multiplicative inverse of a
 */
template<
  typename T,
  typename U
>
static inline typename std::common_type<T, U>::type
modinv(T a, U mod) noexcept
{
  static_assert(
    std::is_integral<T>::value && std::is_integral<U>::value,
    "[modinv] T and U must be integer type");

  return (mod + extgcd(a, mod).first % mod) % mod;
}


/*!
 * @brief Calculate n! mod m while avoiding overflow
 *
 * @tparam T  Integer type for target integer
 * @tparam U  Integer type for modulo
 * @param [in] n    Target integer
 * @param [in] mod  Modulo
 *
 * @return n! mod m
 */
template<
  typename T,
  typename U
>
static inline typename std::common_type<T, U>::type
modfact(T n, U mod) noexcept
{
  static_assert(
    std::is_integral<T>::value && std::is_integral<U>::value,
    "[modfact] T and U must be an integer type");

  typename std::common_type<T, U>::type p = 1;
  for (; n > 0; n--) {
    p = (p * n) % mod;
  }
  return p;
}


/*!
 * @brief Calculate a ** p mod m while avoiding overflow
 *
 * @tparam T  Integer type for base
 * @tparam U  Integer type for exponent
 * @tparam V  Integer type for modulo
 * @param [in] a    Base
 * @param [in] p    Exponent
 * @param [in] mod  Modulo
 *
 * @return a ** p mod m
 */
template<
  typename T,
  typename U,
  typename V
>
static inline typename std::common_type<T, V>::type
powmod(T a, U p, V mod) noexcept
{
  static_assert(
    std::is_integral<T>::value && std::is_integral<U>::value && std::is_integral<V>::value,
    "[powmod] T, U and V must be an integer type");

  typename std::common_type<T, V>::type ans = 1;
  for (; p > 0; p >>= 1, a = (a * a) % mod) {
    if ((p & 1) == 1) {
      ans = (ans * a) % mod;
    }
  }
  return ans;
}


/*!
 * @brief Euler's totient function
 * Calculate the number of disjoint integers i s.t. 1 <= i <= n
 *
 * @tparam T  Integer type
 * @param [in] n    Upper limit
 *
 * @return  The number of disjoint integers
 */
template<typename T>
static inline int
eulerTotient(T n) noexcept
{
  static_assert(std::is_integral<T>::value, "[eulerTotient] T must be an integer type");

  int nDisjoint = n;
  for (T i = 2; i * i <= n; i++) {
    if (n % i == 0) {
      nDisjoint = nDisjoint / i * (i - 1);
      while (n % i == 0) {
        n /= i;
      }
    }
  }
  if (n > 1) {
    nDisjoint = nDisjoint / n * (n - 1);
  }
  return nDisjoint;
}


#endif  // INTEGER_HPP
