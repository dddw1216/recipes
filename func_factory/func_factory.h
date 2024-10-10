// Copyright (c) 2024, dddw1216. All rights reserved.

#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <thread>
#include <mutex>
#include <unistd.h>

namespace recipes::func_factory {

class FuncFactory {
 public:
  FuncFactory() : stop_(false) {
    th_ = std::thread(&FuncFactory::BackgroundRun, this);
  }

  ~FuncFactory() {
    stop_ = true;
    th_.join();
    std::cout << "FuncFactory destoryed!" << std::endl;
  }
  
  static FuncFactory& GetInstance() {
    static FuncFactory ins;
    return ins;
  }

  template <class F, class... Args>
  void ResigistFunc(const std::string& funcname, F&& f, Args&&... args) {
    std::unique_lock<std::mutex> _(mutex_);
    functor_map_[funcname] = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
    return;
  }

 private:
  void BackgroundRun() {
    uint32_t loop_cnt = 0;
    while (!stop_) {
      sleep(1);
      {
        std::unique_lock<std::mutex> _(mutex_);
        std::cout << "loop_cnt=" << loop_cnt << std::endl;
        std::cout << "func_cnt=" << functor_map_.size() << std::endl;
        for (auto& [funcname, functor] : functor_map_) {
          std::cout << "begin run func(" << funcname << ")" << std::endl;
          functor();
        }
        std::cout << "finish run all functor" << std::endl;
        std::cout << "**********************" << std::endl << std::endl;
      }
      loop_cnt++;
    }
    return;
  }

  bool stop_ = true;
  std::thread th_;
  std::mutex mutex_;
  std::unordered_map<std::string, std::function<void()>> functor_map_;

 private:
  FuncFactory(const FuncFactory&) = delete;
  FuncFactory(FuncFactory&&) = delete;
  FuncFactory& operator=(const FuncFactory&) = delete;
  FuncFactory& operator=(FuncFactory&&) = delete;
};

} // namespace recipes::func_factory