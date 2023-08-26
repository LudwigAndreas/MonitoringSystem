//
// Created by Ludwig Andreas on 26.08.2023.
//

#include "ExecutorService.h"

namespace s21 {

ExecutorService::ExecutorService(int numThreads) : stop_flag_(false) {
  for (int i = 0; i < numThreads; ++i) {
    workers_.emplace_back([this]() {
      while (!stop_flag_) {
        std::function<void()> task;
        {
          std::unique_lock<std::mutex> lock(queue_mutex_);
          condition_.wait(lock,
                          [this] {
                            return this->stop_flag_
                                || !this->task_queue_.empty();
                          });
          if (stop_flag_ && task_queue_.empty()) {
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

ExecutorService::~ExecutorService() {
  {
    std::lock_guard<std::mutex> lock(queue_mutex_);
    stop_flag_ = true;
  }
  condition_.notify_all();
  for (std::thread &worker: workers_) {
    if (worker.joinable()) {
      worker.join();
    }
  }
}

}