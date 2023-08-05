//
// Created by Ludwig Andreas on 04.08.2023.
//

#include "MetricScheduler.h"

#include <thread>

namespace s21 {

void MetricScheduler::RegisterMetric(Metric metric, size_t interval_s) {
 schedule[metric.GetName()] = {metric, interval_s};
}

void MetricScheduler::Run() {
  while (true) {
    for (const auto& [metric_name, data] : schedule) {
      auto metric = data.metric;
      auto interval = data.interval;
      if (std::chrono::system_clock::now() >= data.next_execution_time) {
        data.metric.Run();
        data.next_execution_time = std::chrono::system_clock::now() + std::chrono::seconds(interval);
      }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time_ms_));
  }
}
}