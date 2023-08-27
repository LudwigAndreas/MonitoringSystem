//
// Created by Ludwig Andreas on 30.07.2023.
//

#include "Core.h"

#include <utility>
#include <fstream>

namespace s21::monitor {

Core::Core(std::string agents_dir, std::string metric_output_dir) : log_dir_(metric_output_dir){
  app_logger_ = diagnostic::Logger::getRootLogger();
  if (s_instance_) {
    LOG_FATAL(app_logger_, "Only one instance of Core allowed.");
    std::exit(0);
  }
  s_instance_ = this;
  ConfigureMetricLogger();

  agent_manager_ = std::make_shared<AgentManager>(agents_dir);
  metric_analyzer_ = std::make_shared<MetricAnalyzer>(metric_output_dir, metric_logger_);
  agent_executor_ = std::make_shared<AgentExecutor>(metric_analyzer_);
  LOG_INFO(app_logger_, "Agents monitoring started.");
  agent_manager_->Subscribe(agent_executor_.get());
  agent_manager_->StartMonitoring();
}

Core::~Core() {
  agent_manager_->StopMonitoring();
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
  agent_manager_->StartMonitoring();
}

void Core::DisableMonitoring() {
  agent_manager_->StopMonitoring();
}

Core* Core::s_instance_ = nullptr;

}