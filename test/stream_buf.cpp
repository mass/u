#include "u/stream_buf.hpp"

//TODO: Probably use a real testing framework
//TODO: Add actual tests

int main(int argc, char** argv)
{
  UNUSED(argc);
  UNUSED(argv);

  u::stream_buf<char> buf;

  buf.write("Hello, world");
  if (buf.get_read_left() != 12)
    return EXIT_FAILURE;

  buf.erase_read(buf.get_read() + 2, 3);
  if (buf.get_read_view() != "He, world")
    return EXIT_FAILURE;

  buf.advance_read(9);
  if (buf.get_read_left() != 0)
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
