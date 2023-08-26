//
// Created by Ludwig Andreas on 04.08.2023.
//

#include "MetricScheduler.h"

#include <thread>

#include "Logger.h"

namespace s21 {

MetricScheduler::MetricScheduler(ExecutorServicePtr executor) {
  executor_ = executor;
}

MetricScheduler::~MetricScheduler() {}

void MetricScheduler::RegisterAgentBundle(const AgentBundlePtr& agent) {
  schedule_.insert({agent->GetAgentName(), {agent, {}}});
  for (const auto& metric : *agent->GetMetrics()) {
      schedule_[agent->GetAgentName()].metrics.emplace_back(MetricData{metric});
  }
}

void MetricScheduler::Run() {
  while (true) {
    for (const auto& [agent_name, agent] : schedule_) {
      for (const auto& metric_data : agent.metrics) {
        auto interval = metric_data.metric->GetUpdateTime();
        if (std::chrono::system_clock::now() >= metric_data.next_execution_time && executor_ != nullptr) {
          executor_->Submit([] {
//            std::string out = metric_data.metric->Run();
            LOG_TRACE(diagnostic::Logger::getRootLogger(),
                      "Thread " << std::this_thread::get_id())
//                      << " executed metric " << metric_data.metric->GetName()
//                      << " with output " << out);
          });
          metric_data.next_execution_time = std::chrono::system_clock::now() + std::chrono::seconds(interval);
        }
      }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time_ms_));
  }
}

void MetricScheduler::SetSleepTime(size_t sleep_time) {
  sleep_time_ms_ = sleep_time;
}

}