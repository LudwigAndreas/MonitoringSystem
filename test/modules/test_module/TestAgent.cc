//
// Created by Ludwig Andreas on 31.07.2023.
//

#include "TestAgent.h"

#include "test_funcs.h"

namespace s21 {

TestAgent::TestAgent() {}

TestAgent::~TestAgent() {}

std::vector<Metric> TestAgent::GetMetrics() {
  std::vector<Metric> metrics;
  metrics.emplace_back("test_thing", my_test_func);
  metrics.emplace_back("test_thing2", second_test_func);
  metrics.emplace_back("test_thing3", third_test_func);
  return metrics;
}

std::string TestAgent::GetAgentName() {
  return "test agent name";
}

std::string TestAgent::GetAgentType() {
  return "test agent type";
}

}