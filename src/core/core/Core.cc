//
// Created by Ludwig Andreas on 30.07.2023.
//

#include "core.h"

#include <utility>
#include <fstream>

namespace s21::monitor {

Core::Core(const std::string &agents_dir, const std::string &metric_output_dir, size_t update_time)
    : log_dir_(metric_output_dir) {
  app_logger_ = diagnostic::Logger::getRootLogger();
  if (s_instance_) {
    LOG_FATAL(app_logger_, "Only one instance of Core allowed.");
    std::exit(1);
  }
  s_instance_ = this;
  ConfigureMetricLogger();

  agent_manager_ = std::make_shared<AgentManager>(agents_dir, update_time);
  metric_analyzer_ =
      std::make_shared<MetricAnalyzer>(metric_output_dir, metric_logger_);
  agent_executor_ = std::make_shared<AgentExecutor>(metric_analyzer_);
  LOG_INFO(app_logger_, "Agents monitoring started.");
  agent_manager_->Subscribe(agent_executor_.get());
}

Core::~Core() {
  agent_manager_->StopMonitoring();
  LOG_INFO(app_logger_, "Agents monitoring finished.");
}

void Core::ConfigureMetricLogger() {
  metric_logger_ = diagnostic::Logger::getLogger("MonitoringSystem");
  metric_logger_->SetPatternLayout(diagnostic::PatternLayout("%m"));
}

void Core::EnableMonitoring() {
  agent_manager_->StartMonitoring();
}

void Core::DisableMonitoring() {
  agent_manager_->StopMonitoring();
}

void Core::SubscribeAgentEvents(IAgentSubscriber *subscriber) {
  agent_manager_->Subscribe(subscriber);
}

void Core::UnsubscribeAgentEvents(IAgentSubscriber *subscriber) {
  agent_manager_->Unsubscribe(subscriber);
}

void Core::SubscribeMetricEvents(IMetricSubscriber *subscriber) {
  metric_analyzer_->Subscribe(subscriber);
}

void Core::UnsubscribeMetricEvents(IMetricSubscriber *subscriber) {
  metric_analyzer_->Unsubscribe(subscriber);
}

void Core::DeleteAgent(std::shared_ptr<AgentBundle> &agent) {
  agent_manager_->DeleteAgent(agent);
}

Core *Core::s_instance_ = nullptr;

}