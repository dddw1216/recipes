// Copyright (c) 2024, dddw1216. All rights reserved.

#include "func_factory/func_factory.h"

void Func1(double a, double b) {
  std::cout << "In Func1 with a=" << a << ", b=" << b << std::endl;
}

void Func2(const std::string& s) {
  std::cout << "In Func2 with s=" << s << std::endl;
}

void Func3(int x, int y, int z) {
  std::cout << "In Func3 with x=" << x << ", y=" << y << ", z=" << z << std::endl;
}

int main() {

  recipes::func_factory::FuncFactory::GetInstance().ResigistFunc("Func1", Func1, 1.0, 2.0);
  recipes::func_factory::FuncFactory::GetInstance().ResigistFunc("Func2", Func2, "only support return void");
  recipes::func_factory::FuncFactory::GetInstance().ResigistFunc("Func3", Func3, 5, 6, 7);

  sleep(100);
  std::cout << "main out after sleep 100" << std::endl;
  return 0;
}