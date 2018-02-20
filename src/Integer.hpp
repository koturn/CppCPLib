/*!
 * @file Integer.hpp
 * @brief Utility functions about integers
 * @author koturn
 */
#ifndef INTEGER_HPP
#define INTEGER_HPP

#include <cmath>
#include <algorithm>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <vector>


/*!
 * @brief Identify specified integer is prime or not.
 * @tparam T  Integer type
 * @param [in] n  Integer to identify prime or not
 * @return  Return true if specified integer is true, otherwise return false
 */
template<typename T>
static inline bool
isPrime(T n) noexcept
{
  static_assert(std::is_integral<T>::value, "[isPrime] Type of the argument must be an integer");

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
 * @tparam T  Integer type
 * @param [in] n  Upper limit
 * @return  std::vector of prime table
 */
template<typename T>
static inline std::vector<bool>
makePrimeTable(T n) noexcept
{
  static_assert(std::is_integral<T>::value, "[makePrimeTable] Type of the argument must be an integer");

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
 * @brief Make prime list
 * @tparam T  Integer type
 * @param [in] n  Upper limit
 * @return  std::vector of prime list (2, 3, 5, 7, 11...)
 */
template<typename T>
static inline std::vector<T>
makePrimeList(T n) noexcept
{
  static_assert(std::is_integral<T>::value, "[makePrimeList] Type of the argument must be an integer");

  if (n < 0) {
    return std::vector<T>();
  } else if (n == 0) {
    return std::vector<T>{0};
  }
  std::vector<T> primeList(n + 1);
  std::iota(std::begin(primeList), std::end(primeList), 0);
  for (int i = 2; i * i <= n; i++) {
    if (primeList[i] != 0) {
      for (int j = i * 2; j <= n; j += i) {
        primeList[i] = 0;
      }
    }
  }
  primeList.erase(
    std::remove(
      std::begin(primeList),
      std::end(primeList),
      0),
    std::end(primeList));
  return primeList;
}


/*!
 * @brief Defactorize specified integer
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
  static_assert(std::is_integral<T>::value, "[defactorize] Type of the first argument must be an integer");

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
 * @tparam T  Integer type
 * @param [in] n  An integer
 * @return  std::unordered_map of prime factors of specified integer
 */
template<typename T>
static inline std::unordered_map<T, int>
defactorize(T n) noexcept
{
  static_assert(std::is_integral<T>::value, "[defactorize] Type of the argument must be an integer");

  std::unordered_map<T, int> primeFactors;
  defactorize(n, [&primeFactors](T p, int cnt){
    primeFactors[p] = cnt;
  });
  return primeFactors;
}


/*!
 * @brief Calculate divisors of specified integer
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
  static_assert(std::is_integral<T>::value, "[divisors] Type of the first argument must be an integer");

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
  static_assert(std::is_integral<T>::value, "[divisors] Type of the argument must be an integer");

  std::vector<T> divisors;
  divisors(n, [&divisors](const T& v){
    divisors.push_back(v);
  });
  std::sort(std::begin(divisors), std::end(divisors));
  return divisors;
}


/*!
 * @brief Calculate Greatest Common Divisor
 * @tparam T  Integer type
 * @param [in] a  First integer
 * @param [in] b  Second integer
 * @return G.C.D. of a and b
 */
template<typename T>
static inline T
gcd(T a, T b) noexcept
{
  static_assert(std::is_integral<T>::value, "[gcd] Type of the arguments must be an integer");

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
 * @tparam T  Integer type
 * @param [in] a  First integer
 * @param [in] b  Second integer
 * @return L.C.M. of a and b
 */
template<typename T>
static inline T
lcm(T a, T b) noexcept
{
  static_assert(std::is_integral<T>::value, "[lcm] Type of the arguments must be an integer");

#if __cplusplus >= 201703L
  return std::lcm(a, b);  // <numeric>
#else
  return a / gcd(a, b) * b;
#endif  // __cplusplus >= 201703L
}


/*!
 * @brief Determine if two integers are coprime or not.
 * @tparam T  Integer type for first argument
 * @tparam U  Integer type for second argument
 * @param [in] a  First signed integer
 * @param [in] b  Second signed integer
 * @return True if a and b are coprime, otherwise false
 */
template<
  typename T,
  typename U
>
static inline bool
coprime(T a, U b) noexcept
{
  static_assert(std::is_integral<T>::value, "[coprime] Type of the first argument must be an integer");
  static_assert(std::is_integral<T>::value, "[coprime] Type of the second argument must be an integer");

  return gcd(a, b) == 1;
}


/*!
 * @brief Extended Euclidean algorithm (Non-recursive version)
 *
 * Calculate (x, y) s.t. ax + by = gcd(a, b)
 *
 * @tparam T  Integer type for first argument
 * @tparam U  Integer type for second argument
 * @tparam R  Signed integer type for third argument
 * @tparam S  Signed integer type for fourth argument
 * @param [in]  a  First input parameter
 * @param [in]  b  Second input parameter
 * @param [out] x  First output parameter
 * @param [out] y  Second output parameter
 * @return  G.C.D. of a and b
 */
template<
  typename T,
  typename U,
  typename R,
  typename S
>
static inline T
extgcd(T a, U b, R& x, S& y) noexcept
{
  static_assert(std::is_integral<T>::value, "[extgcd] Type of the first argument must be an integer");
  static_assert(std::is_integral<U>::value, "[extgcd] Type of the second argument must be an integer");
  static_assert(std::is_signed<R>::value, "[extgcd] Type of the third argument must be a signed integer");
  static_assert(std::is_signed<S>::value, "[extgcd] Type of the fourth argument must be a signed integer");

  x = 1;
  y = 0;
  for (R u = 0, v = 1; b != 0; ) {
    const auto q = a / b;
    std::swap(u, x -= q * u);
    std::swap(v, y -= q * v);
    std::swap(b, a -= q * b);
  }
  return a;
}


/*!
 * @brief Extended Euclidean algorithm (Non-recursive version)
 *
 * Calculate (x, y) s.t. ax + by = gcd(a, b)
 *
 * @tparam T  Integer type for first argument
 * @tparam U  Integer type for second argument
 * @tparam R  Signed integer type for x and y
 * @param [in]  a  First input parameter
 * @param [in]  b  Second input parameter
 * @return  std::tuple<R, R> of G.C.D., x and y
 */
template<
  typename T,
  typename U,
  typename R = typename std::make_signed<typename std::common_type<T, U>::type>::type
>
static inline std::tuple<T, R, R>
extgcd(T a, U b) noexcept
{
  static_assert(std::is_integral<T>::value, "[extgcd] Type of the first argument must be an integer");
  static_assert(std::is_integral<U>::value, "[extgcd] Type of the second argument must be an integer");

  R x = 1;
  R y = 0;
  auto g = extgcd(a, b, x, y);
  return std::make_tuple(g, x, y);
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
  static_assert(std::is_integral<T>::value, "[modinv] Type of the first argument must be an integer");
  static_assert(std::is_integral<U>::value, "[modinv] Type of the second argument must be an integer");

  const auto t = extgcd(a, mod);
  return std::get<0>(t) == 1 ? (std::get<1>(t) + mod) % mod : 0;
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
  static_assert(std::is_integral<T>::value, "[modfact] Type of the first argument must be an integer");
  static_assert(std::is_integral<U>::value, "[modfact] Type of the second argument must be an integer");

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
modpow(T a, U p, V mod) noexcept
{
  static_assert(std::is_integral<T>::value, "[modpow] Type of the first argument must be an integer");
  static_assert(std::is_integral<U>::value, "[modpow] Type of the second argument must be an integer");

  typename std::common_type<T, V>::type ans = 1;
  for (; p > 0; p >>= 1, a = (a * a) % mod) {
    if ((p & 1) == 1) {
      ans = (ans * a) % mod;
    }
  }
  return ans;
}


/*!
 * @brief Calculate an integer k s.t. x ** k mod m == y
 *
 * @tparam T  Integer type for x
 * @tparam U  Integer type for y
 * @tparam V  Integer type for mod
 * @param [in] x    Base
 * @param [in] y    Remnant
 * @param [in] mod  Modulo
 *
 * @return k s.t. x ** k mod m == y. If k is not exist, return -1
 */
template<
  typename T,
  typename U,
  typename V
>
int
modlog(T x, U y, V mod) noexcept
{
  using C = typename std::common_type<T, V>::type;

  static_assert(std::is_integral<T>::value, "[logmod] Type of the first argument must be an integer");
  static_assert(std::is_integral<U>::value, "[logmod] Type of the second argument must be an integer");
  static_assert(std::is_integral<V>::value, "[logmod] Type of the third argument must be an integer");

  const int h = static_cast<int>(std::sqrt(mod) + 1);
  std::vector<std::pair<C, int>> baby(h);
  C xby = y;
  for (int b = 0; b < h; b++) {
    baby[b] = std::make_pair(xby, b);
    xby = (xby * x) % mod;
  }
  std::sort(std::begin(baby), std::end(baby));

  C xH = 1;
  for (int i = 0; i < h; i++) {
    xH = (xH * x) % mod;
  }
  C xaH = xH;
  for (int a = 1; a <= h; a++, xaH = (xaH * xH) % mod) {
    auto itr = std::lower_bound(std::begin(baby), std::end(baby), std::make_pair(xaH + 1, 0));
    if (itr == std::begin(baby)) {
      continue;
    }
    itr--;
    if (itr->first == xaH) {
      return a * h - itr->second;
    }
  }
  return -1;
}


/*!
 * @brief Euler's totient function
 *
 * Calculate the number of disjoint integers i s.t. 1 <= i <= n
 *
 * @tparam T  Integer type
 * @param [in] n    Upper limit
 * @return  The number of disjoint integers
 */
template<typename T>
static inline int
eulerTotient(T n) noexcept
{
  static_assert(std::is_integral<T>::value, "[eulerTotient] Type of the first argument must be an integer");

  int nDisjoint = n;
  for (T i = 2; i * i <= n; i++) {
    if (n % i == 0) {
      nDisjoint -= nDisjoint / i;
      while (n % i == 0) {
        n /= i;
      }
    }
  }
  if (n > 1) {
    nDisjoint -= n;
  }
  return nDisjoint;
}


// Calculate m s.t. coprime(a, n) == 1 and a^m == 1 (mod n)
template<typename T>
static inline T
carmichaelLambda(T n) noexcept
{
  static_assert(std::is_integral<T>::value, "[carmichaelLambda] Type of the first argument must be an integer"); 
  if (n % 8 == 0) {
    n /= 2;
  }

  T ans = 1;
  for (T i = 2; i <= n; i++) {
    if (n % i == 0) {
      T y = i - 1;
      n /= i;
      while (n % i == 0) {
        n /= i;
        y *= i;
      }
      ans = lcm(ans, y);
    }
  }
  return ans;
}


template<typename T>
static inline int
mobiusMu(T n) noexcept
{
  static_assert(std::is_integral<T>::value, "[mobiusMu] Type of the first argument be an integer");

  int sign = 1;
  for (T i = 2; i <= n; i++) {
    if (n % (i * i) == 0) {
      return 0;
    } else if (n % i == 0) {
      n /= i;
      sign *= -1;
    }
  }
  return sign;
}


#endif  // INTEGER_HPP
