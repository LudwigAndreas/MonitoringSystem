//
// Created by Ludwig Andreas on 04.08.2023.
//

#include "agent_scheduler.h"

#include <thread>

#include "logger.h"

namespace s21 {

AgentScheduler::AgentScheduler(AgentBundlePtr &agent, MetricAnalyzerPtr &analyzer)
    : running_(false), analyzer_(analyzer), agent_(agent){
  for (const auto &metric: *agent->GetMetrics()) {
    metrics_.push_back({metric});
  }
  if (metrics_.empty()) LOG_WARN(diagnostic::Logger::getRootLogger(),
                                 "AgentScheduler: No metrics found for agent: "
                                     << agent->GetAgentName());
  scheduler_thread_ = std::thread(&AgentScheduler::Run, this, 500);
}

AgentScheduler::~AgentScheduler() {
  Stop();
  if (scheduler_thread_.joinable())
    scheduler_thread_.join();
}

void AgentScheduler::Run(size_t sleep_time_ms) {
  agent_->SetUptimePoint(std::chrono::system_clock::now());
  running_ = true;
  std::stringstream ss;
  while (running_ && agent_->IsEnabled()) {
    for (auto &metric_data: metrics_) {
      auto interval = metric_data.metric->GetUpdateTime();
      if (std::chrono::system_clock::now() >= metric_data.scheduled_time) {
        time_t timestamp =
            std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::string value = metric_data.metric->Run();
        analyzer_->Log(MetricEvent(metric_data.metric, value, timestamp));
        metric_data.scheduled_time =
            std::chrono::system_clock::now() + std::chrono::seconds(interval);
      }
    }
    if (!running_ || !agent_->IsEnabled())
      break;
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time_ms));
  }
}

void AgentScheduler::Stop() {
  if (running_)
    running_ = false;
}

}