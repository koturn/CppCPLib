#ifndef BITS_HPP
#define BITS_HPP

#include <cstdint>
#ifdef _MSC_VER
#  include <intrin.h>
#endif


#if defined(__GNUC__)
static int
popcnt(std::uint8_t n)
{
  return __builtin_popcount(n);
}

static int
popcnt(std::uint16_t n)
{
  return __builtin_popcount(n);
}

static int
popcnt(std::uint32_t n)
{
  return __builtin_popcount(n);
}

static int
popcnt(std::uint64_t n)
{
  return __builtin_popcountll(n);
}

#elif defined(_MSC_VER)
static int
popcnt(std::uint8_t n)
{
  return __popcnt16(n);
}

static int
popcnt(std::uint16_t n)
{
  return __popcnt16(n);
}

static int
popcnt(std::uint32_t n)
{
  return __popcnt(n);
}

static int
popcnt(std::uint64_t n)
{
  return __popcnt64(n);
}
#endif  // __GNUC__

// 0x55555555 = 0b01010101010101010101010101010101
// 0x33333333 = 0b00110011001100110011001100110011
// 0x0f0f0f0f = 0b00001111000011110000111100001111
// 0x00ff00ff = 0b00000000111111110000000011111111
// 0x0000ffff = 0b00000000000000001111111111111111

static int
popcnt(std::uint8_t n)
{
  n = (n & 0x55u) + (n >> 1 & 0x55u);
  n = (n & 0x33u) + (n >> 2 & 0x33u);
  return (n & 0x0fu) + (n >> 4 & 0x0fu);
}

static int
popcnt(std::uint16_t n)
{
  n = (n & 0x5555u) + (n >> 1 & 0x5555u);
  n = (n & 0x3333u) + (n >> 2 & 0x3333u);
  n = (n & 0x0f0fu) + (n >> 4 & 0x0f0fu);
  return (n & 0x00ffu) + (n >> 8 & 0x00ffu);
}

static int
popcnt(std::uint32_t n)
{
  n = (n & 0x55555555u) + (n >> 1 & 0x55555555u);
  n = (n & 0x33333333u) + (n >> 2 & 0x33333333u);
  n = (n & 0x0f0f0f0fu) + (n >> 4 & 0x0f0f0f0fu);
  n = (n & 0x00ff00ffu) + (n >> 8 & 0x00ff00ffu);
  return (n & 0x0000ffffu) + (n >> 16 & 0x0000ffffu);
}

static int
popcnt(std::uint64_t n)
{
  n = (n & 0x5555555555555555ull) + (n >> 1 & 0x5555555555555555ull);
  n = (n & 0x3333333333333333ull) + (n >> 2 & 0x3333333333333333ull);
  n = (n & 0x0f0f0f0f0f0f0f0full) + (n >> 4 & 0x0f0f0f0f0f0f0f0full);
  n = (n & 0x00ff00ff00ff00ffull) + (n >> 8 & 0x00ff00ff00ff00ffull);
  n = (n & 0x0000ffff0000ffffull) + (n >> 16 & 0x0000ffff0000ffffull);
  return (n & 0x00000000ffffffffull) + (n >> 32 & 0x00000000ffffffffull);
}

static int
popcnt(std::int8_t n)
{
  return popcnt(static_cast<std::uint8_t>(n));
}

static int
popcnt(std::int16_t n)
{
  return popcnt(static_cast<std::uint16_t>(n));
}

static int
popcnt(std::int32_t n)
{
  return popcnt(static_cast<std::uint32_t>(n));
}

static int
popcnt(std::int64_t n)
{
  return popcnt(static_cast<std::uint64_t>(n));
}




#if defined(__GNUC__)
static int
bsf(std::uint8_t n)
{
  return __builtin_ffs(n) - 1;
}

static int
bsf(std::uint16_t n)
{
  return __builtin_ffs(n) - 1;
}

static int
bsf(std::uint32_t n)
{
  return __builtin_ffs(n) - 1;
}

static int
bsf(std::uint64_t n)
{
  return __builtin_ffsll(n) - 1;
}

#elif defined(_MSC_VER)
static int
bsf(std::uint8_t n)
{
  int index;
  unsigned char isNonZero = _BitScanForward(reinterpret_cast<unsigned long *>(&index), n);
  return isNonZero ? index : -1;
}

static int
bsf(std::uint16_t n)
{
  int index;
  unsigned char isNonZero = _BitScanForward(reinterpret_cast<unsigned long *>(&index), n);
  return isNonZero ? index : -1;
}

static int
bsf(std::uint32_t n)
{
  int index;
  unsigned char isNonZero = _BitScanForward(reinterpret_cast<unsigned long *>(&index), n);
  return isNonZero ? index : -1;
}

static int
bsf(std::uint64_t n)
{
  int index;
  unsigned char isNonZero = _BitScanForward64(reinterpret_cast<unsigned long *>(&index), n);
  return isNonZero ? index : -1;
}
#endif  // defined(__GNUC__)

static int
bsf(std::uint8_t n)
{
  if (n == 0) {
    return -1;
  } else {
    n |= (n << 1);
    n |= (n << 2);
    n |= (n << 4);
    return popcnt(static_cast<std::uint8_t>(~n));
  }
}

static int
bsf(std::uint16_t n)
{
  if (n == 0) {
    return -1;
  } else {
    n |= (n << 1);
    n |= (n << 2);
    n |= (n << 4);
    n |= (n << 8);
    return popcnt(static_cast<std::uint16_t>(~n));
  }
}

static int
bsf(std::uint32_t n)
{
  if (n == 0) {
    return -1;
  } else {
    n |= (n << 1);
    n |= (n << 2);
    n |= (n << 4);
    n |= (n << 8);
    n |= (n << 16);
    return popcnt(~n);
  }
}

static int
bsf(std::uint64_t n)
{
  if (n == 0) {
    return -1;
  } else {
    n |= (n << 1);
    n |= (n << 2);
    n |= (n << 4);
    n |= (n << 8);
    n |= (n << 16);
    n |= (n << 32);
    return popcnt(~n);
  }
}


static int
bsf(std::int8_t n)
{
  return bsf(static_cast<std::uint8_t>(n));
}

static int
bsf(std::int16_t n)
{
  return bsf(static_cast<std::uint16_t>(n));
}

static int
bsf(std::int32_t n)
{
  return bsf(static_cast<std::uint32_t>(n));
}

static int
bsf(std::int64_t n)
{
  return bsf(static_cast<std::uint64_t>(n));
}




#if defined(__GNUC__)
static int
bsr(std::uint8_t n)
{
  return n == 0 ? -1 : (((__builtin_clz(n) & 0x07u) ^ 0x07u));
}

static int
bsr(std::uint16_t n)
{
  return n == 0 ? -1 : (((__builtin_clz(n) & 0x0fu) ^ 0x0fu));
}

static int
bsr(std::uint32_t n)
{
  return n == 0 ? -1 : (__builtin_clz(n) ^ 0x1fu);
}

static int
bsr(std::uint64_t n)
{
  return n == 0 ? -1 : (__builtin_clzll(n) ^ 0x3fu);
}

#elif defined(_MSC_VER)
static int
bsr(std::uint8_t n)
{
  int index;
  unsigned char isNonZero = _BitScanReverse(reinterpret_cast<unsigned long *>(&index), n);
  return isNonZero ? index : -1;
}

static int
bsr(std::uint16_t n)
{
  int index;
  unsigned char isNonZero = _BitScanReverse(reinterpret_cast<unsigned long *>(&index), n);
  return isNonZero ? index : -1;
}

static int
bsr(std::uint32_t n)
{
  int index;
  unsigned char isNonZero = _BitScanReverse(reinterpret_cast<unsigned long *>(&index), n);
  return isNonZero ? index : -1;
}

static int
bsr(std::uint64_t n)
{
  int index;
  unsigned char isNonZero = _BitScanReverse64(reinterpret_cast<unsigned long *>(&index), n);
  return isNonZero ? index : -1;
}
#endif  // defined(__GNUC__)

static int
bsr(std::uint8_t n)
{
  if (n == 0) {
    return -1;
  } else {
    n |= (n >> 1);
    n |= (n >> 2);
    n |= (n >> 4);
    return popcnt(n) - 1;
  }
}

static int
bsr(std::uint16_t n)
{
  if (n == 0) {
    return -1;
  } else {
    n |= (n >> 1);
    n |= (n >> 2);
    n |= (n >> 4);
    n |= (n >> 8);
    return popcnt(n) - 1;
  }
}

static int
bsr(std::uint32_t n)
{
  if (n == 0) {
    return -1;
  } else {
    n |= (n >> 1);
    n |= (n >> 2);
    n |= (n >> 4);
    n |= (n >> 8);
    n |= (n >> 16);
    return popcnt(n) - 1;
  }
}

static int
bsr(std::uint64_t n)
{
  if (n == 0) {
    return -1;
  } else {
    n |= (n >> 1);
    n |= (n >> 2);
    n |= (n >> 4);
    n |= (n >> 8);
    n |= (n >> 16);
    n |= (n >> 32);
    return popcnt(n) - 1;
  }
}


static int
bsr(std::int8_t n)
{
  return bsr(static_cast<std::uint8_t>(n));
}

static int
bsr(std::int16_t n)
{
  return bsr(static_cast<std::uint16_t>(n));
}

static int
bsr(std::int32_t n)
{
  return bsr(static_cast<std::uint32_t>(n));
}

static int
bsr(std::int64_t n)
{
  return bsr(static_cast<std::uint64_t>(n));
}




#endif  // BITS_HPP
