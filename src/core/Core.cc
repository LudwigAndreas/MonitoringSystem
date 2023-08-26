//
// Created by Ludwig Andreas on 30.07.2023.
//

#include "Core.h"

#include <utility>
#include <fstream>

namespace s21::monitor {

Core::Core(std::string agents_dir, std::string metric_output_dir) : log_dir_(std::move(metric_output_dir)), agent_manager_(std::move(agents_dir)){
  app_logger_ = diagnostic::Logger::getRootLogger();
  if (s_instance_) {
    LOG_FATAL(app_logger_, "Only one instance of Core allowed.");
    std::exit(0);
  }
  ConfigureMetricLogger();
  s_instance_ = this;
  LOG_INFO(app_logger_, "Agents monitoring started.");
  auto executor_service = std::make_shared<ExecutorService>(5);
  metric_scheduler_ = std::make_shared<MetricScheduler>(executor_service);
  metric_scheduler_thread_ = std::thread(&MetricScheduler::Run, metric_scheduler_);
  agent_manager_.Subscribe(this);
  agent_manager_.StartMonitoring();
}

Core::~Core() {
  agent_manager_.StopMonitoring();
  LOG_INFO(app_logger_, "Agents monitoring finished.");
}

Core *Core::Instance() {
    if (!s_instance_) {
        LOG_FATAL(Core::s_instance_->app_logger_, "Core not initialized.");
        std::exit(0);
    }
  return s_instance_;
}

void Core::ConfigureMetricLogger() {
  metric_logger_ = diagnostic::Logger::getLogger("MonitoringSystem");
  metric_logger_->SetPatternLayout(diagnostic::PatternLayout("[%d{%Y-%m-%d %X}] | %m%n"));
}

void Core::EnableMonitoring() {
  agent_manager_.StartMonitoring();
}

void Core::DisableMonitoring() {
  agent_manager_.StopMonitoring();
}

Core* Core::s_instance_ = nullptr;

void Core::OnAgentAdded(std::shared_ptr<AgentBundle> agent) {
  metric_scheduler_->RegisterAgentBundle(agent);
}

void Core::OnAgentRemoved(std::shared_ptr<AgentBundle> agent) {
  (void) agent;
//  metric_scheduler_.UnregisterAgentBundle(agent);
//  LOG_TRACE(app_logger_, "Agent removed from scheduler: " + agent->GetAgentName());
}

void Core::OnAgentUpdated(std::shared_ptr<AgentBundle> agent) {
  (void) agent;
//  metric_scheduler_.UpdateAgentBundle(agent);
//  LOG_TRACE(app_logger_, "Agent updated in scheduler: " + agent->GetAgentName());
}

}