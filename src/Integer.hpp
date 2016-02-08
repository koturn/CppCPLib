#ifndef INTEGER_H
#define INTEGER_H

#include <cmath>
#include <iostream>


template<typename IntType>
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


template<typename IntType>
static std::map<IntType, int>
defactorize(IntType n)
{
  std::map<IntType, int> primeFactors;
  int cnt = 0;
  for (; n % 2 == 0; n /= 2, cnt++);
  if (cnt != 0) {
    primeFactors[2] = cnt;
  }
  for (IntType i = 3, iMax = static_cast<IntType>(std::sqrt(n)) + 2; i <= iMax; i += 2) {
    cnt = 0;
    for (; n % i == 0; n /= i, cnt++);
    if (cnt != 0) {
      primeFactors[i] = cnt;
      iMax = std::sqrt(n) + 2;
    }
  }
  if (n != 1) {
    primeFactors[n] = 1;
  }
  return primeFactors;
}


#endif  // INTEGER_H
