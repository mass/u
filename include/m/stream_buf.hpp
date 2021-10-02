#pragma once

#include "m.hpp"

#include <algorithm>
#include <cstring>
#include <utility>

namespace m {

//////////////////////////////////////////////////////////////////////////////
/// Contiguous memory buffer with read/write position tracking
class stream_buf
{
  public:

    stream_buf();
    stream_buf(size_t reserve);
    ~stream_buf();
    stream_buf(stream_buf&& o);

    void clear();
    void free();

    inline const uint8_t*  get_read()      const { return _read; }
    inline size_t          get_read_left() const { return _write - _read; }
    inline m::byte_view    get_read_view() const { return m::byte_view(_read, _write - _read); }
    void advance_read(size_t n);

    inline uint8_t*        get_write()                   { return _write; }
    inline const uint8_t*  get_write()      const        { return _write; }
    inline size_t          get_write_left() const        { return (_buffer + _size) - _write; }
    inline void            write(m::byte_view data)      { write(data.data(), data.size()); }
    inline void            write(std::string_view data)  { write(data.data(), data.size()); }
    void advance_write(size_t n);
    void reserve_write(size_t n);
    void write(const void* data, size_t n);

  private:

    stream_buf(const stream_buf& o) = delete;
    stream_buf& operator=(const stream_buf& o) = delete;
    stream_buf& operator=(stream_buf&& o) = delete;

    uint8_t*  _buffer  = nullptr;
    size_t    _size    = 0;
    uint8_t*  _read    = nullptr;
    uint8_t*  _write   = nullptr;
};

/// Implementation ///////////////////////////////////////////////////////////

inline stream_buf::stream_buf()
  : stream_buf(256)
{}

inline stream_buf::stream_buf(size_t reserve)
{
  _size    = reserve;
  _buffer  = (uint8_t*) ::malloc(_size);
  _read    = _buffer;
  _write   = _buffer;
}

inline stream_buf::~stream_buf()
{
  free();
}

inline stream_buf::stream_buf(stream_buf&& o)
{
  _buffer  = std::exchange(o._buffer,  nullptr);
  _size    = std::exchange(o._size,    0);
  _read    = std::exchange(o._read,    nullptr);
  _write   = std::exchange(o._write,   nullptr);
}

inline void stream_buf::clear()
{
  _read   = _buffer;
  _write  = _buffer;
}

inline void stream_buf::free()
{
  if (_buffer != nullptr)
    ::free(_buffer);
  _buffer  = nullptr;
  _size    = 0;
  _read    = nullptr;
  _write   = nullptr;
}

inline void stream_buf::advance_read(size_t n)
{
  if (auto l = get_read_left(); UNLIKELY(n > l))
    n = l;

  _read += n;

  if (get_read_left() == 0) {
    _read   = _buffer;
    _write  = _buffer;
  }
}

inline void stream_buf::advance_write(size_t n)
{
  if (auto l = get_write_left(); UNLIKELY(n > l))
    n = l;

  _write += n;
}

inline void stream_buf::reserve_write(size_t n)
{
  if (n <= get_write_left())
    return;

  const size_t used = get_read_left();
  _size = 2UL * (n + used);

  uint8_t* newBuf = (uint8_t*) ::malloc(_size);
  ::memcpy(newBuf, _read, used);
  ::free(_buffer);

  _buffer  = newBuf;
  _read    = _buffer;
  _write   = _buffer + used;
}

inline void stream_buf::write(const void* data, size_t n)
{
  reserve_write(n);
  ::memcpy(get_write(), data, n);
  advance_write(n);
}

};
