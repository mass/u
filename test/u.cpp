#include "u/u.hpp"

#include "u/log.hpp"

//TODO: Probably use a real testing framework
//TODO: Add actual tests

int main(int argc, char** argv)
{
  UNUSED(argc);
  UNUSED(argv);

  const auto now = u::now();
  if (now == -1)
    return EXIT_FAILURE;
  LOG(INFO) << "current time in nanos is " << now;

  return EXIT_SUCCESS;
}
