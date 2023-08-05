//
// Created by Ludwig Andreas on 31.07.2023.
//

#include "Metric.h"

namespace s21 {

s21::Metric::Metric(std::string name,
                    std::function<std::string()> metric_func) {
  name_ = name;
  metric_func_ = metric_func;
}

std::string Metric::GetName() const {
  return name_;
}

std::string Metric::Run() const {
  return metric_func_();
}

}