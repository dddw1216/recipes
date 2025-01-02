// Copyright (c) 2024, dddw1216. All rights reserved.

// Command:
// g++ normal.cc -o normal -O2

// Output:
// ./normal
// Sum: 2.79997e+10
// Normal function execution time: 0.00130494 s
// ./jit
// Sum: 2.79997e+10
// JIT Execution time: 0.00106156 s

#include <chrono>
#include <iostream>

// 普通C++版本计算
double DynamicComplexFunc(double x) {
  double result = x;
  for (int i = 1; i <= 10; ++i) {
    result += i * x;
  }
  return result;
}

int main() {
  const int N = 100000;
  double sum = 0;

  // Execution start time
  auto exec_start = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < N; ++i) {
    sum += DynamicComplexFunc(i * 0.1);
  }

  // Execution end time
  auto exec_end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> exec_diff = exec_end - exec_start;

  std::cout << "Sum: " << sum << std::endl;
  std::cout << "Normal function execution time: " << exec_diff.count() << " s\n";

  return 0;
}