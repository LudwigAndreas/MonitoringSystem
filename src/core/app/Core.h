//
// Created by Ludwig Andreas on 30.07.2023.
//

#ifndef MONITORINGSYSTEM_SRC_CORE_CORE_H_
#define MONITORINGSYSTEM_SRC_CORE_CORE_H_

#include <string>

#include "Logger.h"
#include "../../modules/include/Agent.h"
#include "agent/AgentManager.h"
#include "executor/AgentScheduler.h"
#include "executor/AgentExecutor.h"
#include "analyzer/MetricAnalyzer.h"

namespace s21::monitor {

typedef std::shared_ptr<AgentManager> AgentManagerPtr;
typedef std::shared_ptr<AgentExecutor> AgentExecutorPtr;
typedef std::shared_ptr<MetricAnalyzer> MetricAnalyzerPtr;

class Core {

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

 private:

  /**
   * Configures the metric analyzer.
   *
   * analyzer will be configured in the following way:
   * 1. Get the "MonitoringSystem" analyzer.
   * 2. Set the pattern layout to "[%d{%Y-%m-%d %X}] | %m%n".
   * This will print the date and time in the following format: "2023-07-30 12:00:00".
   *
   * */
  void ConfigureMetricLogger();

  static Core* s_instance_;
  std::string log_dir_;
  AgentManagerPtr agent_manager_;
  MetricAnalyzerPtr metric_analyzer_;
  AgentExecutorPtr agent_executor_;
  diagnostic::LoggerPtr metric_logger_;
  diagnostic::LoggerPtr app_logger_;
};

}

#endif //MONITORINGSYSTEM_SRC_CORE_CORE_H_
