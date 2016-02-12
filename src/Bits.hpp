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


/*!
 * @brief Count flagged bits of specified integer (64 bit)
 *
 * @param [in] n  An integer
 *
 * @return The number of flagged bits of specified integer
 */
static std::uint64_t
popcnt(std::uint64_t n)
{
  n = ((n & 0xaaaaaaaaaaaaaaaaul) >> 1) + (n & 0x5555555555555555ul);
  n = ((n & 0xccccccccccccccccul) >> 2) + (n & 0x3333333333333333ul);
  n = ((n & 0xf0f0f0f0f0f0f0f0ul) >> 4) + (n & 0x0f0f0f0f0f0f0f0ful);
  n = ((n & 0xff00ff00ff00ff00ul) >> 8) + (n & 0x00ff00ff00ff00fful);
  n = ((n & 0xffff0000ffff0000ul) >> 16) + (n & 0x0000ffff0000fffful);
  n = ((n & 0xffffffff00000000ul) >> 32) + (n & 0x00000000fffffffful);
  return n;
}
#endif


#endif  // BITS_HPP
