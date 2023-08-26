//
// Created by Ludwig Andreas on 26.08.2023.
//

#ifndef MONITORINGSYSTEM_EXECUTORSERVICE_H_
#define MONITORINGSYSTEM_EXECUTORSERVICE_H_

#include <iostream>
#include <thread>
#include <future>
#include <vector>
#include <queue>

namespace s21 {

class ExecutorService {
 public:
  ExecutorService(int numThreads);

  template<typename Func, typename... Args>
  std::future<std::invoke_result_t<Func, Args...>> Submit(Func f, Args... args) {
    using ReturnType = typename std::invoke_result_t<Func, Args...>;
    std::packaged_task<ReturnType()> task(std::bind(f, args...));
    std::future<ReturnType> res(task.get_future());
    {
      std::lock_guard<std::mutex> lock(queue_mutex_);
      task_queue_.push([&task]() { task(); });
    }
    condition_.notify_one();
    return res;
  }


  ~ExecutorService();

 private:
  ExecutorService() = default;

  std::vector<std::thread> workers_;
  std::queue<std::function<void()>> task_queue_;
  std::mutex queue_mutex_;
  std::condition_variable condition_;
  bool stop_flag_ = false;
};

}

#endif //MONITORINGSYSTEM_EXECUTORSERVICE_H_
