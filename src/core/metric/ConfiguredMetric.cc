//
// Created by Ludwig Andreas on 24.08.2023.
//

#include "ConfiguredMetric.h"

#include "Logger.h"

namespace s21 {

ConfiguredMetric::ConfiguredMetric(const Metric &metric) : Metric(metric), update_time_(60) {}

std::string ConfiguredMetric::Run() {
  if (metric_func_) {
    return metric_func_();
  } else if (metric_func_args_) {
    return metric_func_args_(args_);
  } else {
    LOG_ERROR(diagnostic::Logger::getRootLogger(), "Metric function not set for metric: " + name_);
    return "ERROR";
  }
}

std::string ConfiguredMetric::GetName() const {
  return name_;
}

void ConfiguredMetric::SetName(const std::string &name) {
 name_ = name;
}

const std::string &ConfiguredMetric::GetArgs() const {
  return args_;
}

void ConfiguredMetric::SetArgs(const std::string &args) {
  args_ = args;
}

const MetricCriticalValue &ConfiguredMetric::GetCriticalValue() const {
  return critical_value_;
}

void ConfiguredMetric::SetCriticalValue(const MetricCriticalValue &critical_value) {
  critical_value_ = critical_value;
}

size_t ConfiguredMetric::GetUpdateTime() const {
  return update_time_;
}

void ConfiguredMetric::SetUpdateTime(size_t update_time) {
  update_time_ = update_time;
}

}
