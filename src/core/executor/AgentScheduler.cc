//
// Created by Ludwig Andreas on 04.08.2023.
//

#include "AgentScheduler.h"

#include <thread>

#include "Logger.h"

namespace s21 {

AgentScheduler::AgentScheduler(AgentBundlePtr agent, MetricAnalyzerPtr analyzer)
    : running_(false), analyzer_(analyzer) {
  for (const auto &metric: *agent->GetMetrics()) {
    metrics_.push_back({metric});
  }
  if (metrics_.empty()) LOG_WARN(diagnostic::Logger::getRootLogger(),
                                 "AgentScheduler: No metrics found for agent: "
                                     << agent->GetAgentName())
  scheduler_thread_ = std::thread(&AgentScheduler::Run, this, 500);
}

AgentScheduler::~AgentScheduler() {
  Stop();
}

void AgentScheduler::Run(size_t sleep_time_ms) {
  running_ = true;
  std::stringstream ss;
  while (running_) {
    for (auto &metric_data: metrics_) {
      auto interval = metric_data.metric->GetUpdateTime();
      if (std::chrono::system_clock::now() >= metric_data.scheduled_time) {
        std::string value = metric_data.metric->Run();
        time_t timestamp =
            std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        analyzer_->Log(MetricEvent(metric_data.metric, value, timestamp));
        metric_data.scheduled_time =
            std::chrono::system_clock::now() + std::chrono::seconds(interval);
      }
    }
    if (!running_)
      break;
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time_ms));
  }
}

void AgentScheduler::Stop() {
  running_ = false;
  if (scheduler_thread_.joinable())
    scheduler_thread_.join();
}

}