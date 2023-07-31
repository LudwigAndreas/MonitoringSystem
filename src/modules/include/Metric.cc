//
// Created by Ludwig Andreas on 31.07.2023.
//

#include "Metric.h"

namespace s21 {

s21::Metric::Metric(std::string name,
                    std::function<std::string()> metric_func) {
  _name = name;
  _metric_func = metric_func;
}

std::string Metric::GetName() const {
  return _name;
}

std::string Metric::run() const {
  return _metric_func();
}

}