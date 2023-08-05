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
  agent_manager_.StartMonitoring();
  LOG_INFO(app_logger_, "Agents monitoring started.");
}

Core::~Core() {
  agent_manager_.StopMonitoring();
  LOG_INFO(app_logger_, "Agents monitoring finished.");
}

Core *Core::Instance() {
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

}