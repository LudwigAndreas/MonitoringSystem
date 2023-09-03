//
// Created by Ludwig Andreas on 24.08.2023.
//

#ifndef MONITORINGSYSTEM_SRC_CORE_METRICLOGGER_H_
#define MONITORINGSYSTEM_SRC_CORE_METRICLOGGER_H_

#include <string>
#include <iostream>
#include <fstream>

#include "Logger.h"

#include "agent/agent_bundle.h"
#include "metric/MetricEvent.h"
#include "notifier/IMetricSubscriber.h"

namespace s21 {

namespace fs = std::filesystem;

class MetricAnalyzer {

 public:

  /**
   * Creates a MetricAnalyzer that logs to the given directory.
   *
   * @param logger The analyzer to use.
   *
   * @note metric_output_dir will be set to "./logs/".
   * */
  MetricAnalyzer(diagnostic::LoggerPtr logger);

  /**
   * Creates a MetricAnalyzer that logs to the given directory.
   *
   * @param metric_output_dir The directory to log to.
   *
   * @param logger The analyzer to use.
   *
   * */
  MetricAnalyzer(std::string metric_output_dir, diagnostic::LoggerPtr logger);

  ~MetricAnalyzer();

  void Log(const MetricEvent& event);

  void Subscribe(IMetricSubscriber* notifier);

  void Unsubscribe(IMetricSubscriber* notifier);

 protected:
  std::string log_dir_;
  std::ofstream log_file_;
  std::string current_log_file_;
  diagnostic::LoggerPtr logger_;
  int last_log_day_;
  std::vector<IMetricSubscriber*> notifiers_;
  std::time_t last_log_time_;
  std::stringstream last_log_line;

 private:
  MetricAnalyzer();

  void OpenLogFile(time_t timestamp);
  void CheckForNewDay(time_t timestamp);

  void NotifyCriticalValueReached(const MetricEvent& event);

  void NotifyLineLogged(const std::string& line);

  void NotifyNewFileOpened(const std::string& log_file);
};

}
#endif //MONITORINGSYSTEM_SRC_CORE_METRICLOGGER_H_
