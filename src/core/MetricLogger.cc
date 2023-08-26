//
// Created by Ludwig Andreas on 24.08.2023.
//

#include "MetricLogger.h"

namespace s21 {

MetricLogger::MetricLogger(std::string metric_output_dir) {
  log_dir_ = metric_output_dir;
  logger_ = diagnostic::Logger::getRootLogger();
}

MetricLogger::MetricLogger(diagnostic::LoggerPtr logger) {
  logger_ = logger;
  log_dir_ = "./logs/";
}

MetricLogger::MetricLogger(std::string metric_output_dir,
                           diagnostic::LoggerPtr logger) {
  log_dir_ = metric_output_dir;
  logger_ = logger;
}

MetricLogger::~MetricLogger() {

}

void MetricLogger::Log(MetricEvent event) {
  std::string out = event.GetMetric()->GetName() + " : " + event.GetValue() + " | ";
  LOG_INFO(diagnostic::Logger::getRootLogger(), out);
}

MetricLogger::MetricLogger() {

}

}