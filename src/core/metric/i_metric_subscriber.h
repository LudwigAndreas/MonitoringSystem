//
// Created by Ludwig Andreas on 27.08.2023.
//

#ifndef MONITORINGSYSTEM_IMETRICSUBSCRIBER_H_
#define MONITORINGSYSTEM_IMETRICSUBSCRIBER_H_

#include "metric/metric_event.h"

namespace s21 {

class IMetricSubscriber {

 public:

  virtual void OnCriticalValueReached(MetricEvent event) = 0;

  virtual void OnLineLogged(const std::string& line) = 0;

  virtual void OnNewFileOpened(const std::string& log_file) = 0;

  virtual ~IMetricSubscriber() = default;

};

}

#endif //MONITORINGSYSTEM_IMETRICSUBSCRIBER_H_
