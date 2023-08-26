//
// Created by Ludwig Andreas on 31.07.2023.
//

#ifndef MONITORINGSYSTEM_SRC_CORE_AGENTMANAGER_H_
#define MONITORINGSYSTEM_SRC_CORE_AGENTMANAGER_H_

#include <vector>
#include <filesystem>
#include <thread>

#include "Logger.h"

#include "../../modules/include/Agent.h"
#include "agent_bundle.h"
#include "MetricScheduler.h"
#include "IAgentSubscriber.h"

namespace s21 {

typedef std::shared_ptr<AgentBundle> AgentBundlePtr;

/**
 * The AgentManager is used to manage the life cycle of agents.
 * Monitoring occurs in a separate thread and adds / removes
 * agents from the agent_list.
 * */
class AgentManager {
 public:

  /**
   * Main AgentManager constructor.
   *
   * @param agents_directory specifies the directory in which the appearance
   * of agents is traced
   * */
  AgentManager(std::string agents_directory = "./agents/");

   /**
    * Destructor.
    * */
  ~AgentManager();

  /**
   * Starts agents monitoring in agents_directory_
   * and adds agents to agent_list_
   * */
  void StartMonitoring();

  /**
   * Stops agents monitoring in agents_directory_
   * */
  void StopMonitoring();

  /**
   * Returns the list of agents.
   * */
  std::map<std::string, AgentBundlePtr>* GetAgentList();

  void Subscribe(IAgentSubscriber* subscriber);

  void Unsubscribe(IAgentSubscriber* subscriber);

 protected:

  /**
  * The main method of monitoring operation.
  * This method runs in a separate thread
  * and scans the agents_directory_ directory with a sleep_duration_ interval
  */
  void MonitorAgentsDirectory();

  /**
   * Checks the structure of the agent.
   * */
  bool CheckAgentStructure(const std::filesystem::path &agent_path);

  void NotifyAgentAdded(AgentBundlePtr agent);
  void NotifyAgentRemoved(AgentBundlePtr agent);
  void NotifyAgentUpdated(AgentBundlePtr agent);

  std::string agent_extension_ = ".agent";
  size_t sleep_duration_ = 30;
  std::string agents_directory_;
  bool is_monitoring_;
  std::thread monitoring_thread_;
  diagnostic::LoggerPtr app_logger_;
  std::map<std::string, AgentBundlePtr>* agent_list_;
  std::vector<IAgentSubscriber*> subscribers_;
};

}

#endif //MONITORINGSYSTEM_SRC_CORE_AGENTMANAGER_H_