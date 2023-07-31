//
// Created by Ludwig Andreas on 30.07.2023.
//

#include "Core.h"

#include <utility>

namespace s21::monitor {

Core::Core(std::string agents_dir) : agent_manager_(std::move(agents_dir)){
  app_logger_ = diagnostic::Logger::getRootLogger();
  if (s_instance_) {
    LOG_FATAL(app_logger_, "Only one instance of Core allowed.");
    std::exit(0);
  }
  s_instance_ = this;
  agent_manager_.StartMonitoring();
  LOG_INFO(app_logger_, "Agent monitoring started.")
}

Core::~Core() {
  agent_manager_.StopMonitoring();
  LOG_INFO(app_logger_, "Agent monitoring finished.")
}

Core *Core::Instance() {
  return s_instance_;
}

Core* Core::s_instance_ = nullptr;

}