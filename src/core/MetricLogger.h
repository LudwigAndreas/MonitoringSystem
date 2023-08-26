//
// Created by Ludwig Andreas on 24.08.2023.
//

#ifndef MONITORINGSYSTEM_SRC_CORE_METRICLOGGER_H_
#define MONITORINGSYSTEM_SRC_CORE_METRICLOGGER_H_

#include <string>
#include <filesystem>

#include "Logger.h"

#include "agent/agent_bundle.h"
#include "metric/MetricEvent.h"

namespace s21 {

namespace fs = std::filesystem;

class MetricLogger {

 public:
  /**
   * Creates a MetricLogger that logs to the given directory.
   *
   * @param metric_output_dir The directory to log to.
   *
   * @note The root logger will be used.
   * */
  MetricLogger(std::string metric_output_dir);

  /**
   * Creates a MetricLogger that logs to the given directory.
   *
   * @param logger The logger to use.
   *
   * @note metric_output_dir will be set to "./logs/".
   * */
  MetricLogger(diagnostic::LoggerPtr logger);

  /**
   * Creates a MetricLogger that logs to the given directory.
   *
   * @param metric_output_dir The directory to log to.
   *
   * @param logger The logger to use.
   *
   * */
  MetricLogger(std::string metric_output_dir, diagnostic::LoggerPtr logger);

  ~MetricLogger();

  void Log(MetricEvent event);

 protected:
  std::string log_dir_;
  fs::path log_file_;
  diagnostic::LoggerPtr logger_;

 private:
  MetricLogger();
};

}
#endif //MONITORINGSYSTEM_SRC_CORE_METRICLOGGER_H_
