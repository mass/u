#include "m/stream_buf.hpp"

//TODO: Probably use a real testing framework
//TODO: Add actual tests

int main(int argc, char** argv)
{
  UNUSED(argc);
  UNUSED(argv);

  m::stream_buf buf;
  buf.write("Hello, world");

  if (buf.get_read_left() != 12)
    return EXIT_FAILURE;

  buf.advance_read(12);

  if (buf.get_read_left() != 0)
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
