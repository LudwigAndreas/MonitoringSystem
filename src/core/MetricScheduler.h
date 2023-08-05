//
// Created by Ludwig Andreas on 04.08.2023.
//

#ifndef MONITORINGSYSTEM_SRC_CORE_METRICSCHEDULER_H_
#define MONITORINGSYSTEM_SRC_CORE_METRICSCHEDULER_H_

#include <map>
#include <string>
#include <chrono>

#include "../modules/include/Agent.h"

namespace s21 {

typedef std::shared_ptr<Metric> MetricPtr;

/**
 * MetricScheduler реализует паттер Scheduler и Publisher. Собирает все метрики,
 * и начинает их исполнять в запланированное время.
 *
 **/
class MetricScheduler {

 public:

  void RegisterMetric(Metric metric, size_t interval_s);

  void Run();

 private:
  struct MetricData {
    Metric metric;
    size_t interval;
    mutable std::chrono::system_clock::time_point next_execution_time = std::chrono::system_clock::now();
  };

//  std::map<std::string, AgentData>

  std::map<std::string, MetricData> schedule;
  const size_t sleep_time_ms_ = 500;
};
}

#endif //MONITORINGSYSTEM_SRC_CORE_METRICSCHEDULER_H_
