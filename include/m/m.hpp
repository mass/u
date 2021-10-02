#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <string_view>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
/// Small collection of various C++ utilities.

#define UNUSED(x)   ((void) (x))
#define LIKELY(x)   (__builtin_expect( !!(x), 1))
#define UNLIKELY(x) (__builtin_expect( !!(x), 0))

namespace m {

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
};

// String literal for byte_view
m::byte_view operator"" _bv(const char* d, size_t l) { return m::byte_view((const uint8_t*) d, l); }
