//
// Created by Ludwig Andreas on 04.08.2023.
//

#ifndef MONITORINGSYSTEM_SRC_CORE_METRICSCHEDULER_H_
#define MONITORINGSYSTEM_SRC_CORE_METRICSCHEDULER_H_

#include <map>
#include <string>
#include <chrono>
#include <agent/agent_bundle.h>
#include <executor/ExecutorService.h>

#include "../modules/include/Agent.h"

namespace s21 {


typedef std::shared_ptr<ExecutorService> ExecutorServicePtr;
typedef std::shared_ptr<AgentBundle> AgentBundlePtr;

/**
 * MetricScheduler implements the Scheduler patter. Collects all metrics,
 * and starts executing them at the scheduled time.
 *
 **/
class MetricScheduler {

 public:

  MetricScheduler(ExecutorServicePtr executor);

  ~MetricScheduler();

  /**
   * Registers all metrics from agent.
   **/
  void RegisterAgentBundle(const AgentBundlePtr& agent);

  /**
   * Starts the scheduler.
   **/
  void Run();

  /**
   * Sets the sleep time between each iteration of the scheduler.
   **/
  void SetSleepTime(size_t sleep_time);

 private:
  /**
   * MetricData is a struct that holds the metric and the interval
   * at which it should be executed.
   * */
  struct MetricData {
    ConfiguredMetricPtr metric;
    mutable std::chrono::system_clock::time_point next_execution_time = std::chrono::system_clock::now();
  };

  struct AgentData {
    AgentBundlePtr agent;
    std::vector<MetricData> metrics;
  };

  std::map<std::string, AgentData> schedule_;
  size_t sleep_time_ms_ = 500;
  ExecutorServicePtr executor_;
};
}

#endif //MONITORINGSYSTEM_SRC_CORE_METRICSCHEDULER_H_
