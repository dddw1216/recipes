// Copyright (c) 2024, dddw1216. All rights reserved.

#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <type_traits>
#include <vector>

namespace recipes::thread_pool {

class ThreadTool {
 public:
  ThreadTool(uint32_t thread_num) : stop_(false) {
    for (uint32_t i = 0; i < thread_num; i++) {
      workers_.emplace_back([this] {
        for (;;) {
          std::function<void()> task;
          {
            std::unique_lock<std::mutex> unique_lock(mutex_);
            cv_.wait(unique_lock, [this]() { return stop_ || !task_queue_.empty(); });
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

  template <class F, class... Args>
  std::future<typename std::result_of<F(Args...)>::type> Enqueue(F&& f, Args&&... args) {
    using TypeReturn = typename std::result_of<F(Args...)>::type;
    std::shared_ptr<std::packaged_task<TypeReturn()>> p_task =
        std::make_shared<std::packaged_task<TypeReturn()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    std::future<TypeReturn> res = p_task->get_future();
    {
      std::unique_lock<std::mutex> _(mutex_);
      task_queue_.emplace([p_task]() { (*p_task)(); });
    }
    cv_.notify_one();
    return res;
  }

 private:
  bool stop_ = true;
  std::queue<std::function<void()>> task_queue_;
  std::vector<std::thread> workers_;
  std::mutex mutex_;
  std::condition_variable cv_;
};

}  // namespace recipes::thread_pool
