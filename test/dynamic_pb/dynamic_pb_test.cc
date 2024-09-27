// Copyright (c) 2024, dddw1216. All rights reserved.

#include <iostream>
#include <string>

#include "dynamic_pb/proto/item_node.pb.h"

int main(int argc, char** argv) {
  recipes::ItemNode item_node;
  std::cout << "item_node:\n" << item_node.DebugString() << std::endl;
  return 0;
}