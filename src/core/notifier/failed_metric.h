#pragma once

#include <string>

#include "metric/metric_event.h"

namespace s21 {
struct FailedMetric {
  std::string metric_name;
  std::string critical_value;
  std::string value;
  std::string date;

  FailedMetric(std::string metric_name, std::string critical_value,
               std::string value, std::string date)
      : metric_name(metric_name),
        critical_value(critical_value),
        value(value),
        date(date) {}

  FailedMetric(s21::MetricEvent event) {
    time_t time = event.GetTimestamp();
    metric_name = event.GetMetric()->GetName();
    critical_value =
        event.GetMetric()->GetCriticalValue().GetConditionOperator() +
        std::to_string(
            event.GetMetric()->GetCriticalValue().GetCriticalValue());
    value = event.GetValue();
    date = asctime(gmtime(&time));
  }
};
}  // namespace s21