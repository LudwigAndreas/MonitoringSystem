//
// Created by Ludwig Andreas on 26.08.2023.
//

#ifndef MONITORINGSYSTEM_SRC_CORE_METRIC_METRICCRITICALVALUE_H_
#define MONITORINGSYSTEM_SRC_CORE_METRIC_METRICCRITICALVALUE_H_

#include <string>

namespace s21 {

class MetricCriticalValue {
 public:
  MetricCriticalValue() = default;

  MetricCriticalValue(const std::string &condition_operator, double critical_value);

  MetricCriticalValue(std::string critical_value);
  bool IsCritical(double value) const;

  bool IsCritical(std::string value) const;

  std::string GetConditionOperator() const;

  void SetConditionOperator(const std::string &condition_operator);

  double GetCriticalValue() const;

  void SetCriticalValue(double critical_value);

 private:
  std::string condition_operator_;
  double critical_value_;
};

}

#endif //MONITORINGSYSTEM_SRC_CORE_METRIC_METRICCRITICALVALUE_H_
