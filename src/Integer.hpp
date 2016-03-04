#ifndef INTEGER_HPP
#define INTEGER_HPP

#include <cmath>
#include <algorithm>
#include <map>
#include <type_traits>
#include <vector>


/*!
 * @brief Identify specified integer is prime or not.
 *
 * @tparam IntType  Integer type
 * @param [in] n  Integer to identify prime or not
 * @return  Return true if specified integer is true, otherwise return false
 */
template<typename IntType, typename std::enable_if<std::is_integral<IntType>::value, std::nullptr_t>::type = nullptr>
static bool
isPrime(IntType n)
{
  if (n < 2) {
    return false;
  } else if (n == 2) {
    return true;
  } else if (n % 2 == 0) {
    return false;
  }
  for (IntType i = 3; i * i <= n; i += 2) {
    if (n % i == 0) {
      return false;
    }
  }
  return true;
}


/*!
 * @brief Make prime table
 *
 * @tparam IntType  Integer type
 * @param [in] n  Upper limit
 *
 * @return  std::vector of prime table
 */
template<typename IntType, typename std::enable_if<std::is_integral<IntType>::value, std::nullptr_t>::type = nullptr>
static std::vector<bool>
makePrimeTable(IntType n)
{
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
 * @tparam IntType  Integer type
 * @param [in] n  An integer
 *
 * @return  std::map of prime factors of specified integer
 */
template<typename IntType, typename std::enable_if<std::is_integral<IntType>::value, std::nullptr_t>::type = nullptr>
static std::map<IntType, int>
defactorize(IntType n)
{
  std::map<IntType, int> primeFactors;
  int cnt = 0;
  for (; n % 2 == 0; n /= 2, cnt++);
  if (cnt != 0) {
    primeFactors[2] = cnt;
  }
  for (IntType i = 3; i * i <= n; i += 2) {
    cnt = 0;
    for (; n % i == 0; n /= i, cnt++);
    if (cnt != 0) {
      primeFactors[i] = cnt;
    }
  }
  if (n != 1) {
    primeFactors[n] = 1;
  }
  return primeFactors;
}


/*!
 * @brief Calculate divisors of specified integer
 *
 * @tparam IntType  Integer type
 * @param [in] n  An integer
 *
 * @return  std::vector of divisors of specified integer
 */
template <typename IntType, typename std::enable_if<std::is_integral<IntType>::value, std::nullptr_t>::type = nullptr>
std::vector<IntType>
divisors(IntType n)
{
  std::vector<IntType> divisors;
  for (IntType i = 1; i * i <= n; i++) {
    if (n % i == 0) {
      divisors.push_back(i);
      if (i != n / i) {
        divisors.push_back(n / i);
      }
    }
  }
  std::sort(divisors.begin(), divisors.end());
  return divisors;
}


/*!
 * @brief Extended Euclidean algorithm
 * Calculate (x, y) s.t. ax + by = gcd(a, b)
 *
 * @tparam SignedIntType  Signed integer type
 * @param [in]  a  First input parameter
 * @param [in]  b  Second input parameter
 * @param [out] x  First output parameter
 * @param [out] y  Second output parameter
 *
 * @return  GCD of a and b
 */
template<typename SignedIntType, typename std::enable_if<std::is_signed<SignedIntType>::value, std::nullptr_t>::type = nullptr>
SignedIntType
extgcd(SignedIntType a, SignedIntType b, SignedIntType& x, SignedIntType& y)
{
  if (b == 0) {
    x = 1;
    y = 0;
    return a;
  } else {
    SignedIntType g = extgcd(b, a % b, y, x);
    y -= (a / b) * x;
    return g;
  }
}


/*!
 * @brief Euler's totient function
 * Calculate the number of disjoint integers i s.t. 1 <= i <= n
 *
 * @tparam IntType  Integer type
 * @param [in] n    Upper limit
 *
 * @return  The number of disjoint integers
 */
template<typename IntType>
int eulerTotient(IntType n, typename std::enable_if<std::is_integral<IntType>::value, std::nullptr_t>::type = nullptr)
{
  int nDisjoint = n;
  for (IntType i = 2; i * i <= n; i++) {
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
