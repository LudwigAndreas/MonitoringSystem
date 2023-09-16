//
// Created by Ludwig Andreas on 31.07.2023.
//

#include "bonus_agent.h"

#include "bonus.h"

namespace s21 {

BonusAgent::BonusAgent() {}

BonusAgent::~BonusAgent() {}

std::vector<Metric> BonusAgent::GetMetrics() {
  std::vector<Metric> metrics;
  metrics.emplace_back("cpu_idle_usage", cpu_idle_usage);
  metrics.emplace_back("cpu_user_usage", cpu_user_usage);
  metrics.emplace_back("cpu_privileged_usage", cpu_privileged_usage);
  metrics.emplace_back("cpu_dpc_usage", cpu_dpc_usage);
  metrics.emplace_back("cpu_interrupt_usage", cpu_interrupt_usage);
  metrics.emplace_back("total_swap", total_swap);
  metrics.emplace_back("used_swap", used_swap);
  metrics.emplace_back("proc_queue_length", proc_queue_length);
  metrics.emplace_back("virtual_mem_volume", virtual_mem_volume);
  metrics.emplace_back("virtual_mem_free", virtual_mem_free);
  metrics.emplace_back("inodes", inodes);
  metrics.emplace_back("hard_read_time", hard_read_time);
  metrics.emplace_back("system_errors", system_errors);
  metrics.emplace_back("user_auths", user_auths);
  return metrics;
}

std::string BonusAgent::GetAgentName() {
  return "Bonus agent name";
}

std::string BonusAgent::GetAgentType() {
  return "Bonus agent type";
}

}