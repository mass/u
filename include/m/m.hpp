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

  // Helpers for unsigned char (byte) values
  using byte_view  = std::basic_string_view<uint8_t>;
  using byte_vec   = std::vector<uint8_t>;
  inline byte_view byte_vec_to_view(const byte_vec& vec) { return byte_view(vec.data(), vec.size()); }

};
