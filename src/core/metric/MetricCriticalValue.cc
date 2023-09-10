//
// Created by Ludwig Andreas on 26.08.2023.
//

#include "MetricCriticalValue.h"

#include <string>

#include "Logger.h"

namespace s21 {

MetricCriticalValue::MetricCriticalValue(const std::string &condition_operator,
                                         double critical_value) {
  condition_operator_ = condition_operator;
  critical_value_ = critical_value;
}

MetricCriticalValue::MetricCriticalValue(std::string critical_value) {
  bool is_set = false;
  for (const auto &op: {"!=", "==", ">=", "<=", ">", "<"}) {
    if (critical_value.find(op) != std::string::npos) {
      condition_operator_ = op;
      try {
        critical_value_ =
            std::stod(critical_value.substr(critical_value.find(op) + strlen(op)));
      } catch (std::exception &e) {
        LOG_ERROR(diagnostic::Logger::getRootLogger(),
                  "Critical value is not a number: "
                      + critical_value.substr(critical_value.find(op) + strlen(op)));
        critical_value = 0.;
      }
      is_set = true;
      break;
    }
  }
  if (!is_set) {
    LOG_ERROR(diagnostic::Logger::getRootLogger(),
              "Critical value contains syntax error: " + critical_value);
    critical_value_ = 0.;
    condition_operator_ = "";
  }
}

bool MetricCriticalValue::IsCritical(double value) const {
  if (condition_operator_ == ">") {
    return value > critical_value_;
  } else if (condition_operator_ == "<") {
    return value < critical_value_;
  } else if (condition_operator_ == ">=") {
    return value >= critical_value_;
  } else if (condition_operator_ == "<=") {
    return value <= critical_value_;
  } else if (condition_operator_ == "==") {
    return value == critical_value_;
  } else if (condition_operator_ == "!=") {
    return value != critical_value_;
  }
  return false;
}

bool MetricCriticalValue::IsCritical(std::string value) const {
  if (std::isdigit(value[0])) {
    try {
      return IsCritical(std::stod(value));
    } catch (std::exception &e) {
      LOG_ERROR(diagnostic::Logger::getRootLogger(),
                "Critical value is not a number: " + value);
      return false;
    }
  }
  return false;
}

std::string MetricCriticalValue::GetConditionOperator() const {
  return condition_operator_;
}

void MetricCriticalValue::SetConditionOperator(const std::string &condition_operator) {
  condition_operator_ = condition_operator;
}

double MetricCriticalValue::GetCriticalValue() const {
  return critical_value_;
}

void MetricCriticalValue::SetCriticalValue(double critical_value) {
  critical_value_ = critical_value;
}

std::string MetricCriticalValue::ToString() const {
  std::string critical_value = std::to_string(critical_value_);
  return condition_operator_ + critical_value.substr(0, critical_value.find_last_not_of('0'));
}

}