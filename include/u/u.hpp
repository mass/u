#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <random>
#include <string_view>
#include <time.h>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
/// Small collection of various C++ utilities.

#define UNUSED(x)   ((void) (x))
#define LIKELY(x)   (__builtin_expect( !!(x), 1))
#define UNLIKELY(x) (__builtin_expect( !!(x), 0))

namespace u {

  // Convienence definitions for unsigned char (byte) values
  using byte_view  = std::basic_string_view<uint8_t>;
  using byte_vec   = std::vector<uint8_t>;

  // Create views from containers of other types
  template<typename T, typename F>
  inline std::basic_string_view<T> view(std::basic_string_view<F> v) {
    return std::basic_string_view<T>((const T*) v.data(), v.size());
  }
  template<typename T, typename F>
  inline std::basic_string_view<T> view(const std::basic_string<F>& v) {
    return std::basic_string_view<T>((const T*) v.data(), v.size());
  }
  template<typename T, typename F>
  inline std::basic_string_view<T> view(const std::vector<F>& v) {
    static_assert(sizeof(F) == 1);
    return std::basic_string_view<T>((const T*) v.data(), v.size());
  }
  template<typename T>
  inline std::basic_string_view<T> view(const std::vector<T>& v) {
    static_assert(sizeof(T) == 1);
    return std::basic_string_view<T>((const T*) v.data(), v.size());
  }

  // Return the current time in nanoseconds since the Epoch
  inline int64_t now()
  {
    struct timespec tv;
    if (UNLIKELY(::clock_gettime(CLOCK_REALTIME, &tv) != 0))
      return -1L;
    return (tv.tv_sec * 1000000000L) + tv.tv_nsec;
  }

  // Set variable and return whether it changed
  template<typename T>
  inline constexpr bool chset(T& p, T&& n) noexcept
  {
    if (p == n)
      return false;
    p = std::forward<T>(n);
    return true;
  }

  // Handy wrapper for erase-remove idiom
  template<typename T, typename F>
  inline constexpr void eraseRemove(T& m, F&& f) noexcept
  {
    m.erase(std::remove_if(m.begin(), m.end(), std::forward<F>(f)), m.end());
  }

  // Returns a pseudo-random number on the interval [0,N)
  template<typename T>
  inline T getRand(T n)
  {
    static_assert(std::is_integral_v<T> && not std::is_signed_v<T>);

    static thread_local std::random_device rng{};
    static thread_local std::mt19937 prng{ rng() };

    if (n == 0)
      n = 1;
    std::uniform_int_distribution<T> dist(0, n - 1);

    return dist(prng);
  }

};

// String literal for byte_view
inline u::byte_view operator"" _bv(const char* d, size_t l) { return u::byte_view((const uint8_t*) d, l); }
