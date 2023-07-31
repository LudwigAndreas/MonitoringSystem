//
// Created by Ludwig Andreas on 31.07.2023.
//

#ifndef MONITORINGSYSTEM_SRC_CORE_AGENTMANAGER_H_
#define MONITORINGSYSTEM_SRC_CORE_AGENTMANAGER_H_

#include <vector>

#include "../modules/include/Agent.h"

#include <thread>

#include "Logger.h"

namespace s21 {

typedef void *DynamicLib;

class AgentManager {
 public:
  AgentManager(std::string agents_directory = "./agents/");

  void StartMonitoring();

  void StopMonitoring();

 private:

  void MonitorAgentsDirectory();

  s21::Agent *Instantiate(DynamicLib lib);

//  static const std::string s_lib_entry_point_;
//  static const size_t s_sleep_duration_;
  #if __APPLE__
    const std::string lib_extension_ = ".dylib";
  #elif _WIN32
    const std::string lib_extension_ = ".dll";
  #else
    const std::string lib_extension_ = ".so";
  #endif
  const std::string lib_entry_point_ = "get_agent";
  const size_t sleep_duration_ = 30;
  std::string agents_directory_;
  bool is_monitoring_;
  std::thread monitoring_thread_;
  diagnostic::LoggerPtr app_logger_;
  std::vector<Agent *> agent_list_;
};

}

#endif //MONITORINGSYSTEM_SRC_CORE_AGENTMANAGER_H_
