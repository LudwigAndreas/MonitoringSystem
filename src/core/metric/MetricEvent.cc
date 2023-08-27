//
// Created by Ludwig Andreas on 26.08.2023.
//

#include "MetricEvent.h"

namespace s21 {

MetricEvent::MetricEvent(ConfiguredMetricPtr &metric,
                         std::string &value,
                         time_t &timestamp) {
  metric_ = metric;
  value_ = value;
  timestamp_ = timestamp;
}

MetricEvent::~MetricEvent() {}

const s21::ConfiguredMetricPtr &MetricEvent::GetMetric() const {
  return metric_;
}
const std::string &MetricEvent::GetValue() const {
  return value_;
}
time_t MetricEvent::GetTimestamp() const {
  return timestamp_;
}

}