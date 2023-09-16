#include "network_agent.h"

#include "network.h"

namespace s21 {

NetworkAgent::NetworkAgent() {}

NetworkAgent::~NetworkAgent() {}

std::vector<Metric> NetworkAgent::GetMetrics() {
  std::vector<Metric> metrics;
  metrics.emplace_back("url", url);
  metrics.emplace_back("inet_throughput", inet_throughput);
  return metrics;
}

std::string NetworkAgent::GetAgentName() {
  return "NetworkAgent";
}

std::string NetworkAgent::GetAgentType() {
  return "Network";
}

}