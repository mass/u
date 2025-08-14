#pragma once

#include "u.hpp"

#include <algorithm>
#include <cstring>
#include <utility>

namespace u {

////////////////////////////////////////////////////////////////////////////////
/// Contiguous memory buffer with read/write position tracking
template<typename byte_t = uint8_t>
class stream_buf
{
  using sv_t = std::basic_string_view<byte_t>;

  public:

    stream_buf();
    stream_buf(size_t reserve);
    ~stream_buf();
    stream_buf(stream_buf&& o);
    stream_buf& operator=(stream_buf&& o);

    void clear();
    void free();

    inline const byte_t*  get_read()      const { return _read; }
    inline size_t         get_read_left() const { return _write - _read; }
    inline sv_t           get_read_view() const { return sv_t(_read, _write - _read); }
    void advance_read(size_t n);
    void erase_read(const byte_t* pos, size_t n);

    inline byte_t*  get_write()            { return _write; }
    inline size_t   get_write_left() const { return (_buffer + _size) - _write; }
    inline void     write(sv_t data)       { write(data.data(), data.size()); }
    void advance_write(size_t n);
    void reserve_write(size_t n);
    void write(const void* data, size_t n);

  private:

    static_assert(std::is_same_v<byte_t, char> || std::is_same_v<byte_t, uint8_t>);

    stream_buf(const stream_buf& o) = delete;
    stream_buf& operator=(const stream_buf& o) = delete;

    byte_t*  _buffer  = nullptr;
    size_t   _size    = 0;
    byte_t*  _read    = nullptr;
    byte_t*  _write   = nullptr;
};

/// Implementation /////////////////////////////////////////////////////////////

template<typename byte_t>
inline stream_buf<byte_t>::stream_buf()
  : stream_buf(256)
{}

template<typename byte_t>
inline stream_buf<byte_t>::stream_buf(size_t reserve)
{
  _size    = reserve;
  _buffer  = (byte_t*) ::malloc(_size);
  _read    = _buffer;
  _write   = _buffer;
}

template<typename byte_t>
inline stream_buf<byte_t>::~stream_buf()
{
  free();
}

template<typename byte_t>
inline stream_buf<byte_t>::stream_buf(stream_buf&& o)
{
  _buffer  = std::exchange(o._buffer,  nullptr);
  _size    = std::exchange(o._size,    0);
  _read    = std::exchange(o._read,    nullptr);
  _write   = std::exchange(o._write,   nullptr);
}

template<typename byte_t>
inline stream_buf<byte_t>& stream_buf<byte_t>::operator=(stream_buf&& o)
{
  if (this == &o)
    return *this;
  free();
  _buffer  = std::exchange(o._buffer,  nullptr);
  _size    = std::exchange(o._size,    0);
  _read    = std::exchange(o._read,    nullptr);
  _write   = std::exchange(o._write,   nullptr);
  return *this;
}

template<typename byte_t>
inline void stream_buf<byte_t>::clear()
{
  _read   = _buffer;
  _write  = _buffer;
}

template<typename byte_t>
inline void stream_buf<byte_t>::free()
{
  if (_buffer != nullptr)
    ::free(_buffer);
  _buffer  = nullptr;
  _size    = 0;
  _read    = nullptr;
  _write   = nullptr;
}

template<typename byte_t>
inline void stream_buf<byte_t>::advance_read(size_t n)
{
  if (auto l = get_read_left(); UNLIKELY(n > l))
    n = l;

  _read += n;

  if (get_read_left() == 0) {
    _read   = _buffer;
    _write  = _buffer;
  }
}

template<typename byte_t>
inline void stream_buf<byte_t>::erase_read(const byte_t* pos, size_t n)
{
  if (UNLIKELY(pos < _read || pos >= _write))
    return;

  if (size_t m = _write - pos; UNLIKELY(n > m))
    n = m;

  // Move down bytes from (pos + n) to _write into pos
  if (auto o = _write - (pos + n); o > 0)
    ::memmove((byte_t*) pos, pos + n, o);

  _write -= n;
}

template<typename byte_t>
inline void stream_buf<byte_t>::advance_write(size_t n)
{
  if (auto l = get_write_left(); UNLIKELY(n > l))
    n = l;

  _write += n;
}

template<typename byte_t>
inline void stream_buf<byte_t>::reserve_write(size_t n)
{
  if (n <= get_write_left())
    return;

  const size_t used = get_read_left();
  _size = 2UL * (n + used);

  byte_t* newBuf = (byte_t*) ::malloc(_size);
  ::memcpy(newBuf, _read, used);
  ::free(_buffer);

  _buffer  = newBuf;
  _read    = _buffer;
  _write   = _buffer + used;
}

template<typename byte_t>
inline void stream_buf<byte_t>::write(const void* data, size_t n)
{
  reserve_write(n);
  ::memcpy(get_write(), data, n);
  advance_write(n);
}

};
