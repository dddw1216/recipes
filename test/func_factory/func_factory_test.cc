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

int Func4(char c) {
  std::cout << "In Func4 with c=" << c << std::endl;
  return 0;
}

int main() {

  recipes::func_factory::FuncFactory::GetInstance().ResigistFunc("Func1", Func1, 1.0, 2.0);
  recipes::func_factory::FuncFactory::GetInstance().ResigistFunc("Func2", Func2, "only support return void");
  recipes::func_factory::FuncFactory::GetInstance().ResigistFunc("Func3", Func3, 5, 6, 7);

  /*
  std::function<void()> 是一个函数对象，它可以接受任何可以调用且没有参数并且返回类型可以转换为 void 的对象。
  在你的例子中，Func4 函数的返回类型是 int，但是当你将它绑定到 std::function<void()> 时，它的返回值会被忽略，因此它可以被转换为 void。
  当你调用 ResigistFunc("Func4", Func4, '*'); 时，你实际上是创建了一个新的函数对象，这个函数对象调用 Func4 并传入参数 '*'，然后忽略返回值。
  这个新的函数对象满足 std::function<void()> 的要求，因此它可以被存储在 functor_map_ 中。
  这就是为什么你可以将 Func4 注册到 functor_map_ 中，尽管 Func4 的返回类型是 int 而不是 void。

  是的，你可以将任何返回类型的函数或可调用对象绑定到 std::function<void()>。
  这是因为 std::function<void()> 的实例在调用其包含的函数或可调用对象时会忽略其返回值。
  但是，需要注意的是，如果你的函数或可调用对象需要接收参数，那么你需要确保在绑定到 std::function<void()> 时已经提供了所有必要的参数（例如通过 std::bind 或 lambda 表达式）。
  这是因为 std::function<void()> 表示的是一个不接收任何参数的函数。
  所以，总的来说，任何返回类型的函数或可调用对象都可以绑定到 std::function<void()>，只要它们不需要接收参数，或者在绑定时已经提供了所有必要的参数。
  */
  recipes::func_factory::FuncFactory::GetInstance().ResigistFunc("Func4", Func4, '*');

  sleep(100);
  std::cout << "main out after sleep 100" << std::endl;
  return 0;
}