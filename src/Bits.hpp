#ifndef BITS_HPP
#define BITS_HPP

#include <cstdint>


#ifdef __GNUC__
#  define popcnt(n)  __builtin_popcount(n)
#else
/*!
 * @brief Count flagged bits of specified integer (32 bit)
 *
 * @param [in] n  An integer
 *
 * @return The number of flagged bits of specified integer
 */
static std::uint32_t
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
