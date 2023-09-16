//
// Created by Ludwig Andreas on 31.07.2023.
//

#include "metric.h"

namespace s21 {

s21::Metric::Metric(std::string name,
                    std::function<std::string()> metric_func) {
  name_ = name;
  metric_func_ = metric_func;
  metric_func_args_ = nullptr;
}

Metric::Metric(std::string name,
               std::function<std::string(std::string)> metric_func_args) {
    name_ = name;
    metric_func_args_ = metric_func_args;
    metric_func_ = nullptr;
}

// Metric::Metric(const Metric &metric) {
//   name_ = metric.name_;
//   metric_func_ = metric.metric_func_;
//   metric_func_args_ = metric.metric_func_args_;
// }

}