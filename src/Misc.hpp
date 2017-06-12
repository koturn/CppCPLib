/*!
 * @brief Miscellaneous utilities
 * @author koturn
 * @file Misc.hpp
 */
#ifndef MISC_HPP
#define MISC_HPP

#include <cassert>
#include <cmath>
#include <complex>
#include <limits>
#include <type_traits>
#include <vector>


/*!
 * @brief Round up n to the samllest power of two.
 * @tparam T Integer type
 * @param [in] n  An integer
 * @return argmin p s.t. n ** p
 */
template<typename T>
static inline T
roundUpPowerOfTwo(T n) noexcept
{
  static_assert(std::is_integral<T>::value, "[roundUpPowerOfTwo] Type of the argument must be an integer");
#ifdef assert
  assert(n >= 0);
#endif
  n--;
  for (std::size_t shift = 1; shift <= sizeof(T) * 4; shift <<= 1) {
    n |= n >> shift;
  }
  return n + 1;
}


/*!
 * @brief Fast Fourier Transform
 * @tparam kSign  Sign number (1 or -1) which indicates fft or ifft
 * @tparam Iterator  Iterator of complex sequence
 * @param [in] begin  Start of complex sequence
 * @param [in] end    End of complex sequence
 */
template<
  int kSign = 1,
  typename Iterator
>
static inline void
fft(const Iterator& begin, const Iterator& end) noexcept
{
  static_assert(kSign == 1 || kSign == -1, "[fft] kSign must be 1 or -1");
  using V = typename std::iterator_traits<Iterator>::value_type;
  using size_type = decltype(std::distance(begin, end));

  constexpr V kI(0, 1);
  const auto size = std::distance(begin, end);

#ifdef M_PI
  auto theta = kSign * 2 * M_PI / static_cast<V>(size);
#else
  auto theta = kSign * 2 * std::acos(-1) / static_cast<V>(size);
#endif  // M_PI

  for (auto m = size; m >= 2; m >>= 1) {
    const auto mh = static_cast<size_type>(m >> 1);
    for (size_type i = 0; i < mh; i++) {
      const auto w = std::exp(static_cast<V>(i) * theta * kI);
      for (auto j = i; j < size; j += m) {
        const auto k = j + mh;
        auto& c1 = *std::next(begin, j);
        auto& c2 = *std::next(begin, k);
        const auto x = c1 - c2;
        c1 += c2;
        c2 = w * x;
      }
    }
    theta *= 2.0;
  }

  size_type i = 0;
  for (size_type j = 1; j < size - 1; j++) {
    for (auto k = size >> 1; k > (i ^= k); k >>= 1);
    if (j < i) {
      std::swap(*std::next(begin, i), *std::next(begin, j));
    }
  }
}


/*!
 * @brief Inverse Fast Fourier Transform
 * @tparam Iterator  Iterator of complex sequence
 * @param [in] begin  Start of complex sequence
 * @param [in] end    End of complex sequence
 */
template<typename Iterator>
static inline void
ifft(const Iterator& begin, const Iterator& end) noexcept
{
  using V = typename std::iterator_traits<Iterator>::value_type;

  fft<-1>(begin, end);
  const auto rSize = 1.0 / static_cast<V>(std::distance(begin, end));
  for (auto itr = begin; itr != end; ++itr) {
    *itr *= rSize;
  }
}


/*!
 * @brief Fast Fourier Transform
 * @tparam T  Type of std::complex elements
 * @param [in,out] seq  Complex sequence
 */
template<typename T>
static inline void
fft(std::vector<std::complex<T>>& seq) noexcept
{
  static_assert(std::is_floating_point<T>::value, "[fft] Vector element type must be floating point complex");
  const auto size = roundUpPowerOfTwo(seq.size());
  if (size != seq.size()) {
    seq.resize(size);
  }
  fft(std::begin(seq), std::end(seq));
}


/*!
 * @brief Inverse Fast Fourier Transform
 * @tparam T  Type of std::complex elements
 * @param [in,out] seq  Complex sequence
 */
template<typename T>
static inline void
ifft(std::vector<std::complex<T>>& seq) noexcept
{
  static_assert(std::is_floating_point<T>::value, "[ifft] Vector element type must be floating point complex");
  const auto size = roundUpPowerOfTwo(seq.size());
  if (size != seq.size()) {
    seq.resize(size);
  }
  ifft(std::begin(seq), std::end(seq));
}


/*!
 * @brief Convolution with Fast Fourier Transform
 * @tparam Iterator1  Type of iterator of first complex sequence
 * @tparam Iterator2  Type of iterator of second complex sequence
 * @param [in] begin1  Start of the first complex sequence
 * @param [in] end1    End of the first complex sequence
 * @param [in] begin2  Start of the second complex sequence
 */
template<
  typename Iterator1,
  typename Iterator2
>
static inline void
fftConvolution(const Iterator1& begin1, const Iterator1& end1, const Iterator2& begin2) noexcept
{
  fft(begin1, end1);
  fft(begin2, std::next(begin2, std::distance(begin1, end1)));
  Iterator2 itr2 = begin2;
  for (Iterator1 itr1 = begin1; itr1 != end1; ++itr1, ++itr2) {
    *itr1 *= *itr2;
  }
  ifft(begin1, end1);
}


/*!
 * @brief Convolution with Fast Fourier Transform
 * @tparam T  Type of std::complex elements of first complex sequence
 * @tparam U  Type of std::complex elements of second complex sequence
 * @param [in,out] va  First complex sequence
 * @param [in,out] vb  Second complex sequence
 */
template<
  typename T,
  typename U
>
static inline void
fftConvolution(std::vector<std::complex<T>>& va, std::vector<std::complex<U>>& vb) noexcept
{
  const auto size = roundUpPowerOfTwo(std::max(va.size(), vb.size()));
  va.resize(size);
  vb.resize(size);
  fftConvolution(std::begin(va), std::end(va), std::begin(vb));
}


/*!
 * @brief Floor floating-point number and cast it to integer
 *
 * @tparam R  Destination type (integer)
 * @tparam T  Source type (Floating-point)
 * @param [in] x  A floating-point number
 * @return Floored integer
 */
template<
  typename R,
  typename T
>
static inline R
floor_cast(T x) noexcept
{
  static_assert(std::is_floating_point<R>::value, "[floor_cast] Type of the argument must be a floating point");
  static_assert(std::is_integral<T>::value, "[floor_cast] Type of return value must be a integer");
  return static_cast<R>(std::floor(x));
}


/*!
 * @brief Ceil floating-point number and cast it to integer
 *
 * @tparam R  Destination type (integer)
 * @tparam T  Source type (Floating-point)
 * @param [in] x  A floating-point number
 * @return Ceild integer
 */
template<
  typename R,
  typename T
>
static inline R
ceil_cast(T x) noexcept
{
  static_assert(std::is_floating_point<R>::value, "[ceil_cast] Type of the argument must be a floating point");
  static_assert(std::is_integral<T>::value, "[ceil_cast] Type of return value must be a integer");
  return static_cast<R>(std::ceil(x));
}


/*!
 * @brief Round floating-point number and cast it to integer
 *
 * @tparam R  Destination type (integer)
 * @tparam T  Source type (Floating-point)
 * @param [in] x  A floating-point number
 * @return Rounded integer
 */
template<
  typename R,
  typename T
>
static inline R
round_cast(T x) noexcept
{
  static_assert(std::is_floating_point<T>::value, "[round_cast] Type of the argument must be a floating point");
  static_assert(std::is_integral<R>::value, "[round_cast] Type of return value must be a integer");
  return static_cast<R>(round(x));
}



/*!
 * @brief Clamp number into the range of destination type
 *
 * Destination type must be narrower than source type.
 * Otherwise, use static_cast
 *
 * @tparam R  Destination type (must be narrower than T)
 * @tparam T  Source type
 * @param [in] x  A number
 * @return Saturated number
 */
template<
  typename R,
  typename T
>
static inline R
saturation_cast(T x) noexcept
{
  static_assert(
    (std::is_integral<T>::value && std::is_integral<R>::value && sizeof(T) > sizeof(R))
    || (std::is_floating_point<T>::value && std::is_floating_point<R>::value && sizeof(T) > sizeof(R))
    || (std::is_floating_point<T>::value && std::is_integral<R>::value),
    "[saturation_cast] Destination value range must be narrower than source value range");
  constexpr auto min = std::numeric_limits<R>::min();
  constexpr auto max = std::numeric_limits<R>::max();
  return x < min ? min : x > max ? max : static_cast<R>(x);
}


#endif  // MISC_HPP
