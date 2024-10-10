// Copyright (c) 2024, dddw1216. All rights reserved.

#include "thread_pool/thread_pool.h"

int main() {
  std::vector<std::function<void()>> task_vec;
  for (size_t i = 0; i < 10; i++) {
    task_vec.emplace_back([i]() {
      std::cout << "run task idx=" << i << std::endl;
    });
  }

  /// std::function<void()> and can't not get result
  recipes::thread_pool::ThreadTool thread_pool(2);
  // for (size_t i = 0; i < task_vec.size(); i++) {
  //   thread_pool.Enqueue(task_vec[i]);
  // }

  int a = 1;
  int b = 10;
  /// std::future and std::promise
  auto func = [a, b] (int c) { return a+b+c; };
  std::vector<std::future<int>> result_vec;
  for (int c = 200; c < 500; c += 100) {
    result_vec.emplace_back(std::move(thread_pool.Enqueue(func, c)));
  }

  for (size_t i = 0; i < result_vec.size(); i++) {
    std::cout << "i=" << i << ", result=" << result_vec[i].get() << std::endl;
  }

  return 0;
}
