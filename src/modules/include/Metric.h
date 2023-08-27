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

/**
 * The metric provides an object and some function of the metric.
 * Each Agent contains a list of metrics and executes the run()
 * method for each one.
 *
 * @note Each metric should be independent of the other. Metrics should not
 * depend on Core components.
 *
 * @see Run()
 **/
class Metric {
 protected:
/**
   * The metric must have a unique name
   **/
  std::string name_;
  std::function<std::string()> metric_func_;
  std::function<std::string(std::string)> metric_func_args_;

 public:
  /**
   * Main metric constructor. To create a metric, you must use this constructor
   *
   * @param name unique metrics name
   * @param metric_func function or method that will be executed when Run()
   * is called. The function cannot take any arguments,
   * and returns an not formatted string with the result of execution.
   **/
  Metric(std::string name, std::function<std::string()> metric_func);

  Metric(std::string name, std::function<std::string(std::string)> metric_func_args);

  Metric(const Metric &metric);

  Metric(Metric &&metric) = default;

  Metric() = default;
  virtual ~Metric() = default;
};

}

#endif //MONITORINGSYSTEM_SRC_MODULES_DUMB_MODULE_METRICS_H_
