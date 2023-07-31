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

  Core(std::string agents_dir = "./agents/");

  ~Core();

  Core* Instance();

 private:
  static Core* s_instance_;
  std::vector<Agent*> agent_repo_;
  diagnostic::LoggerPtr metric_logger_;
  diagnostic::LoggerPtr app_logger_;
  AgentManager agent_manager_;
};

}

#endif //MONITORINGSYSTEM_SRC_CORE_CORE_H_
