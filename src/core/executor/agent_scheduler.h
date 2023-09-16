//
// Created by Ludwig Andreas on 04.08.2023.
//

#ifndef MONITORINGSYSTEM_SRC_CORE_AGENTSCHEDULER_H_
#define MONITORINGSYSTEM_SRC_CORE_AGENTSCHEDULER_H_

#include <map>
#include <string>
#include <chrono>
#include <thread>

#include "modules/include/agent.h"
#include "analyzer/metric_analyzer.h"
#include "agent/agent_bundle.h"

namespace s21 {


typedef std::shared_ptr<AgentBundle> AgentBundlePtr;
typedef std::shared_ptr<MetricAnalyzer> MetricAnalyzerPtr;

/**
 * AgentScheduler collects all metrics from the agent,
 * and starts executing them at the scheduled time.
 *
 * The scheduler is a thread that runs infinitely.
 * It is started by calling the Run method.
 * The scheduler can be stopped by calling the Stop method.
 **/
class AgentScheduler {

 public:

  AgentScheduler(AgentBundlePtr &agent, MetricAnalyzerPtr &analyzer);

  ~AgentScheduler();

  /**
   * Starts the scheduler.
   **/
  void Run(size_t sleep_time_ms = 500);

  /**
   * Stops the scheduler.
   **/
  void Stop();

 private:
  AgentScheduler() = default;

  struct MetricData {
    ConfiguredMetricPtr metric;
    mutable std::chrono::system_clock::time_point scheduled_time = std::chrono::system_clock::now();
  };
  bool running_ = false;
  std::thread scheduler_thread_;
  std::vector<MetricData> metrics_;
  MetricAnalyzerPtr analyzer_;
  AgentBundlePtr agent_;
};
}

#endif //MONITORINGSYSTEM_SRC_CORE_AGENTSCHEDULER_H_
