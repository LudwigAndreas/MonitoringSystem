//
// Created by Ludwig Andreas on 31.07.2023.
//

#include "CPUAgent.h"

#include "cpu.h"

namespace s21 {

CPUAgent::CPUAgent() {}

CPUAgent::~CPUAgent() {}

std::vector<Metric> CPUAgent::GetMetrics() {
  std::vector<Metric> metrics;
  metrics.emplace_back("cpu", cpu);
  metrics.emplace_back("processes", processes);
  return metrics;
}

std::string CPUAgent::GetAgentName() {
  return "CPU agent name";
}

std::string CPUAgent::GetAgentType() {
  return "CPU agent type";
}

}