//
// Created by Ludwig Andreas on 24.08.2023.
//

#ifndef MONITORINGSYSTEM_SRC_CORE_METRIC_CONFIGUREDMETRIC_H_
#define MONITORINGSYSTEM_SRC_CORE_METRIC_CONFIGUREDMETRIC_H_

#include "../../modules/include/metric.h"
#include "metric_critical_value.h"

namespace s21 {

/**
 * Inherited class from Metric to add additional information to a metric
 * from the Properties configuration.
 *
 * @see Metric
 * @see Properties
 *
 * */
class ConfiguredMetric : public Metric {
 public:
  /**
   * Constructor for ConfiguredMetric.
   *
   * @param metric The metric that will be configured.
   *
   * */
  ConfiguredMetric();

  ConfiguredMetric(const Metric &metric);

  ConfiguredMetric(const std::string name, const std::string args,
                   const MetricCriticalValue &critical_value,
                   size_t update_time);

  std::string GetName() const;

  void SetName(const std::string &name);

  const std::string &GetArgs() const;

  void SetArgs(const std::string &args);

  const MetricCriticalValue &GetCriticalValue() const;

  void SetCriticalValue(const MetricCriticalValue &critical_value);

  size_t GetUpdateTime() const;

  void SetUpdateTime(size_t update_time);

  std::string Run();  // TODO rename to Execute

 private:
  std::string args_;
  MetricCriticalValue critical_value_;
  size_t update_time_;
};

}  // namespace s21

#endif
