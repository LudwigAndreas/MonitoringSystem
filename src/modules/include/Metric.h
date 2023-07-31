//
// Created by Ludwig Andreas on 30.07.2023.
//

#ifndef MONITORINGSYSTEM_SRC_MODULES_DUMB_MODULE_METRICS_H_
#define MONITORINGSYSTEM_SRC_MODULES_DUMB_MODULE_METRICS_H_

#include <string>
#include <functional>
#include <vector>
#include <map>

namespace s21 {

// @update_time_s - store update time in seconds
// @critical_value and @critical_operator - store critical value
class Metric {
 protected:
  std::string _name;
  std::function<std::string()> _metric_func;

 public:
  Metric(std::string name, std::function<std::string()> metric_func);

  std::string GetName() const;
  std::string run() const;
};

}

#endif //MONITORINGSYSTEM_SRC_MODULES_DUMB_MODULE_METRICS_H_
