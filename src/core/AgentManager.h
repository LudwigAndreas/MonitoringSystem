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

typedef std::shared_ptr<Agent> AgentPtr;
typedef void *DynamicLib;

/**
* The AgentManager is used to manage the life cycle of agents.
* Monitoring occurs in a separate thread and adds / removes
* agents from the agent_list.
*/
class AgentManager {
 public:

  /**
  * Main AgentManager constructor.
  *
  * @param agents_directory specifies the directory in which the appearance
  * of agents is traced
  *
  */
  AgentManager(std::string agents_directory = "./agents/");

  /**
  * Starts agents monitoring in agents_directory_
  */
  void StartMonitoring();

  /**
  * Stops agents monitoring in agents_directory_
  */
  void StopMonitoring();

 protected:

  /**
  * The main method of monitoring operation.
  * This method runs in a separate thread
  * and scans the agents_directory_ directory with a sleep_duration_ interval
  */
  void MonitorAgentsDirectory();

  /**
  * A helper method for getting an agent object from a dynamic library.
  *
  * @param lib pointer to an open dynamic library that has a lib_entry_point method
  *
  * @return std::shared_ptr of Agent on success and nullptr on failure
  */
  std::shared_ptr<s21::Agent> Instantiate(DynamicLib lib);

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
  std::map<std::string, AgentPtr> agent_list_;
};

}

#endif //MONITORINGSYSTEM_SRC_CORE_AGENTMANAGER_H_
