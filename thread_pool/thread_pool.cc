// Copyright (c) 2024, dddw1216. All rights reserved.

#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <functional>
#include <atomic>
#include <mutex>
#include <condition_variable>

namespace recipes::thread_pool {

class ThreadTool {
 public:
  ThreadTool(uint32_t thread_num)  : stop_(false) {
    for (uint32_t i = 0; i < thread_num; i++) {
      workers_.emplace_back([this] {
        for (;;) {
          std::function<void()> task;
          {
            std::unique_lock<std::mutex> unique_lock(mutex_);
            cv_.wait(unique_lock, [this]() { return stop_ || !task_queue_.empty();});
            if (stop_ && task_queue_.empty()) {
              return;
            }
            task = std::move(task_queue_.front());
            task_queue_.pop();
          }
          task();
        }
      });
    }
  }

  ~ThreadTool() {
    {
      std::unique_lock<std::mutex> unque_lock(mutex_);
      stop_ = true;
    }
    cv_.notify_all();
    for (size_t i = 0; i < workers_.size(); i++) {
      workers_[i].join();
    }
  }

  int Enqueue(std::function<void()> task) {
    std::unique_lock<std::mutex> _(mutex_);
    if (stop_) {
      return -1;
    }
    task_queue_.push(std::move(task));
    cv_.notify_one();
    return 0;
  }


private:
  bool stop_ = true;
  std::queue<std::function<void()>> task_queue_;
  std::vector<std::thread> workers_;
  std::mutex mutex_;
  std::condition_variable cv_;
};

} // namespace recipes::thread_pool


int main() {
  std::vector<std::function<void()>> task_vec;
  for (size_t i = 0; i < 10; i++) {
    task_vec.emplace_back([i]() {
      std::cout << "run task idx=" << i << std::endl;
    });
  }

  recipes::thread_pool::ThreadTool thread_pool(2);
  for (size_t i = 0; i < task_vec.size(); i++) {
    thread_pool.Enqueue(task_vec[i]);
  }
  
  return 0;
}
