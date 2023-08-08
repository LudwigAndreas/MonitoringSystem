#include "MemoryAgent.h"

#include "memory.h"

namespace s21 {

MemoryAgent::MemoryAgent() {}

MemoryAgent::~MemoryAgent() {}

std::vector<Metric> MemoryAgent::GetMetrics() {
  std::vector<Metric> metrics;
  metrics.emplace_back("ram_total", ram_total);
  metrics.emplace_back("ram", ram);
  metrics.emplace_back("hard_volume", hard_volume); 
  metrics.emplace_back("hard_ops", hard_ops);
  return metrics;
}

std::string MemoryAgent::GetAgentName() {
  return "MemoryAgent";
}

std::string MemoryAgent::GetAgentType() {
  return "Memory";
}

}