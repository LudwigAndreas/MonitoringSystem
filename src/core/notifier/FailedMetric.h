#pragma once

#include <string>

#include "metric/MetricEvent.h"

struct FailedMetric {
	std::string metric_name;
	std::string critical_value;
	std::string value;
	std::string date;

	FailedMetric(
		std::string metric_name,
		std::string critical_value,
		std::string value,
		std::string date
	) : metric_name(metric_name), 
		critical_value(critical_value), 
		value(value), 
		date(date) {}

    FailedMetric(s21::MetricEvent event) {
      metric_name = event.GetMetric()->GetName();
      critical_value = event.GetMetric()->GetCriticalValue().GetConditionOperator() + std::to_string(event.GetMetric()->GetCriticalValue().GetCriticalValue());
      value = event.GetValue();
      date = std::to_string(event.GetTimestamp());
    }
};