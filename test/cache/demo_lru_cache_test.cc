// Copyright (c) 2024, dddw1216. All rights reserved.

#include "cache/demo_lru_cache.h"

#include <cassert>

int main() {
  recipes::cache::DemoLruCache<uint64_t, std::string> dlc(3);
  std::cout << "dlc to_string 1:\n" << dlc.DebugToString() << std::endl;

  dlc.Put(1, "111");
  dlc.Put(2, "222");
  dlc.Put(3, "333");
  std::cout << "dlc to_string 2:\n" << dlc.DebugToString() << std::endl;

  std::string get;
  dlc.Get(1, get);
  assert(get == "111");
  std::cout << "dlc to_string 3:\n" << dlc.DebugToString() << std::endl;

  dlc.Put(4, "444");
  std::cout << "dlc to_string 4:\n" << dlc.DebugToString() << std::endl;
  return 0;
}