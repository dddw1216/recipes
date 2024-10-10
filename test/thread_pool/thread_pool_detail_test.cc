// Copyright (c) 2024, dddw1216. All rights reserved.

#include <unistd.h>
#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <vector>

class ThreadPool {
 public:
  ThreadPool(size_t);
  template <class F, class... Args>
  auto enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>;
  ~ThreadPool();

 private:
  // need to keep track of threads so we can join them
  std::vector<std::thread> workers;
  // the task queue
  std::queue<std::function<void()> > tasks;

  // synchronization
  std::mutex queue_mutex;
  std::condition_variable condition;
  bool stop;
};

// the constructor just launches some amount of workers
inline ThreadPool::ThreadPool(size_t threads) : stop(false) {
  for (size_t i = 0; i < threads; ++i)
    workers.emplace_back([this] {
      uint64_t loop_cnt = 0;
      for (;;) {
        std::function<void()> task;

        {
          std::unique_lock<std::mutex> lock(this->queue_mutex);
          this->condition.wait(lock, [this] { return this->stop || !this->tasks.empty(); });
          if (this->stop && this->tasks.empty()) return;
          task = std::move(this->tasks.front());
          this->tasks.pop();
          std::cout << __LINE__ << " [ThreadPool] "
                    << "pop a task and then task_queue_size=" << tasks.size() << std::endl;
        }
        std::cout << __LINE__ << " [ThreadPool] "
                  << "begin run task with curr loop_cnt=" << loop_cnt << std::endl;
        task();
        std::cout << __LINE__ << " [ThreadPool] "
                  << "end run task ... " << std::endl;

        loop_cnt++;
      }
    });
}

// add new work item to the pool
template <class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type> {
  using return_type = typename std::result_of<F(Args...)>::type;

  auto task =
      std::make_shared<std::packaged_task<return_type()> >(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
  std::cout << __LINE__ << " [Enqueue] "
            << "task type=" << typeid(task).name() << ", return_type=" << typeid(return_type).name() << std::endl;

  std::future<return_type> res = task->get_future();
  std::cout << __LINE__ << " [Enqueue] "
            << "future res type=" << typeid(res).name() << std::endl;
  {
    std::unique_lock<std::mutex> lock(queue_mutex);

    // don't allow enqueueing after stopping the pool
    if (stop) throw std::runtime_error("enqueue on stopped ThreadPool");

    tasks.emplace([task]() { (*task)(); });
    std::cout << __LINE__ << " [Enqueue] "
              << "put task into task_queue succ" << std::endl;
  }
  condition.notify_one();
  std::cout << __LINE__ << " [Enqueue] "
            << "notity_one worker" << std::endl;
  return res;
}

// the destructor joins all threads
inline ThreadPool::~ThreadPool() {
  {
    std::unique_lock<std::mutex> lock(queue_mutex);
    stop = true;
  }
  condition.notify_all();
  for (std::thread& worker : workers) worker.join();
}

class Test {
 public:
  int PlusA(int x) {
    a += x;
    std::cout << "a=" << a << ", x=" << x << std::endl;
    return a;
  }

  int PlusB(int x) {
    b += x;
    std::cout << "b=" << b << ", x=" << x << std::endl;
    return b;
  }

  int PrintS(std::unique_ptr<std::string> p_str) {
    std::cout << "print_s=" << *p_str << std::endl;
    return -1000;
  }

 public:
  int a{0};
  int b{0};
};

int main() {
  Test t;
  std::unique_ptr<std::string> p_str = std::make_unique<std::string>("xxxxx");
  std::cout << *p_str << std::endl;

  std::unique_ptr<ThreadPool> p_thread_pool_ = std::make_unique<ThreadPool>(16);

  int x = 10;
  p_thread_pool_->enqueue(&Test::PlusB, &t, x);

  /// why??

  // p_thread_pool_->enqueue(&Test::PrintS, &t, std::move(p_str));

  sleep(5);
  return 0;
}
