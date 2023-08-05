//
// Created by Ludwig Andreas on 30.07.2023.
//

#ifndef MONITORINGSYSTEM_SRC_CORE_CORE_H_
#define MONITORINGSYSTEM_SRC_CORE_CORE_H_

#include <string>

#include "Logger.h"
#include "../modules/include/Agent.h"
#include "AgentManager.h"

namespace s21::monitor {

class Core {

 public:

  Core(std::string agents_dir = "./agents/", std::string metric_output_dir = "./logs/");

  ~Core();

  Core* Instance();

  void EnableMonitoring();
  void DisableMonitoring();

 private:

  void ConfigureMetricLogger();

  static Core* s_instance_;
  std::vector<AgentPtr> agent_repo_;
  diagnostic::LoggerPtr metric_logger_;
  diagnostic::LoggerPtr app_logger_;
  std::string log_dir_;
  AgentManager agent_manager_;
};

}

#endif //MONITORINGSYSTEM_SRC_CORE_CORE_H_
