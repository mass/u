#include "m/log.hpp"

//TODO: Probably use a real testing framework
//TODO: Add actual tests

int main(int argc, char** argv)
{
  UNUSED(argc);
  UNUSED(argv);

  LOG(INFO) << "test INFO";
  LOG(WARN) << "test WARN";
  LOG(ERROR) << "test ERROR";
  LOG(INFO) << "test num=" << 1234;
  LOG(INFO) << "test pair " << std::make_pair("key", "val");

  return EXIT_SUCCESS;
}
