//
// Created by Ludwig Andreas on 26.08.2023.
//

#ifndef MONITORINGSYSTEM_METRICEVENT_H_
#define MONITORINGSYSTEM_METRICEVENT_H_

#include <string>
#include <ctime>

#include "core/agent/agent_bundle.h"

namespace s21 {

class MetricEvent {

    public:

    MetricEvent(ConfiguredMetricPtr &metric, std::string &value, std::time_t &timestamp);
    ~MetricEvent();

  const ConfiguredMetricPtr &GetMetric() const;
  const std::string &GetValue() const;
  time_t GetTimestamp() const;

 private:

    ConfiguredMetricPtr metric_;
    std::string value_;
    std::time_t timestamp_;
};

}
#endif //MONITORINGSYSTEM_METRICEVENT_H_
