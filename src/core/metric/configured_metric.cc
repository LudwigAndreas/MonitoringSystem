//
// Created by Ludwig Andreas on 24.08.2023.
//

#include "configured_metric.h"

#include <csignal>

#include "logger.h"

namespace s21 {

ConfiguredMetric::ConfiguredMetric() {
  name_ = "";
  args_ = "";
  critical_value_ = MetricCriticalValue();
  update_time_ = 60;
}

ConfiguredMetric::ConfiguredMetric(const Metric &metric)
    : Metric(metric), update_time_(60) {}

ConfiguredMetric::ConfiguredMetric(const std::string name,
                                   const std::string args,
                                   const MetricCriticalValue &critical_value,
                                   size_t update_time) {
  name_ = name;
  args_ = args;
  critical_value_ = critical_value;
  update_time_ = update_time;
}

void SigAbortHandler(int signum) {
  LOG_FATAL(diagnostic::Logger::getRootLogger(),
            "Metric function aborted with signal: " << signum);
  exit(1);
}

std::string ConfiguredMetric::Run() {
  signal(SIGABRT, &SigAbortHandler);
  signal(SIGSEGV, &SigAbortHandler);
  std::string ret;
  if (metric_func_) {
    ret = metric_func_();
  } else if (metric_func_args_) {
    ret = metric_func_args_(args_);
  } else {
    LOG_ERROR(diagnostic::Logger::getRootLogger(),
              "Metric function not set for metric: " + name_);
    ret = "ERROR";
  }
  signal(SIGABRT, SIG_DFL);
  signal(SIGSEGV, SIG_DFL);
  return ret;
}

std::string ConfiguredMetric::GetName() const { return name_; }

void ConfiguredMetric::SetName(const std::string &name) { name_ = name; }

const std::string &ConfiguredMetric::GetArgs() const { return args_; }

void ConfiguredMetric::SetArgs(const std::string &args) { args_ = args; }

const MetricCriticalValue &ConfiguredMetric::GetCriticalValue() const {
  return critical_value_;
}

void ConfiguredMetric::SetCriticalValue(
    const MetricCriticalValue &critical_value) {
  critical_value_ = critical_value;
}

size_t ConfiguredMetric::GetUpdateTime() const { return update_time_; }

void ConfiguredMetric::SetUpdateTime(size_t update_time) {
  update_time_ = update_time;
}

}  // namespace s21
