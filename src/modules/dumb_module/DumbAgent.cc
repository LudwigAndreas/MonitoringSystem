//
// Created by Ludwig Andreas on 31.07.2023.
//

#include "DumbAgent.h"

#include "dumb_funcs.h"

namespace s21 {

DumbAgent::DumbAgent() {}

DumbAgent::~DumbAgent() {}

std::vector<Metric> DumbAgent::GetMetrics() {
  std::vector<Metric> metrics;
  metrics.push_back(Metric("dumb thing", my_dumb_func));
  return metrics;
}

std::string DumbAgent::GetAgentName() {
  return "dumb agent name";
}

std::string DumbAgent::GetAgentType() {
  return "dumb agent type";
}

}