//
// Created by Ludwig Andreas on 30.07.2023.
//

#ifndef MONITORINGSYSTEM_SRC_CORE_CORE_H_
#define MONITORINGSYSTEM_SRC_CORE_CORE_H_

#include <string>

#include "Logger.h"
#include "../modules/include/Agent.h"
#include "agent/AgentManager.h"
#include "MetricScheduler.h"

namespace s21::monitor {

class Core : public IAgentSubscriber {

 public:

  /**
   * Only one instance of Core allowed.
   *
   * @param agents_dir Directory where the agents are located.
   *
   * @param metric_output_dir Directory where the metric logs are stored.
   *
   * */
  Core(std::string agents_dir = "./agents/", std::string metric_output_dir = "./logs/");

  /**
   * Destructor.
   * */
  ~Core();

  /**
   * Returns the instance of the Core.
   * */
  Core* Instance();

  /**
   * Enables the monitoring of the agents.
   * */
  void EnableMonitoring();

  /**
   * Disables the monitoring of the agents.
   * */
  void DisableMonitoring();

  void OnAgentAdded(std::shared_ptr<AgentBundle> agent) override;
  void OnAgentRemoved(std::shared_ptr<AgentBundle> agent) override;
  void OnAgentUpdated(std::shared_ptr<AgentBundle> agent) override;

 private:

  /**
   * Configures the metric logger.
   *
   * logger will be configured in the following way:
   * 1. Get the "MonitoringSystem" logger.
   * 2. Set the pattern layout to "[%d{%Y-%m-%d %X}] | %m%n".
   * This will print the date and time in the following format: "2023-07-30 12:00:00".
   *
   * */
  void ConfigureMetricLogger();

  static Core* s_instance_;
  std::vector<AgentPtr> agent_repo_;
  diagnostic::LoggerPtr metric_logger_;
  diagnostic::LoggerPtr app_logger_;
  std::string log_dir_;
  AgentManager agent_manager_;
  std::shared_ptr<MetricScheduler> metric_scheduler_;
  std::thread metric_scheduler_thread_;
};

}

#endif //MONITORINGSYSTEM_SRC_CORE_CORE_H_
