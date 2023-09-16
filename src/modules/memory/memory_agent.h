
#ifndef MONITORINGSYSTEM_SRC_MODULES_MEMORY_MODULE_MEMORYAGENT_H_
#define MONITORINGSYSTEM_SRC_MODULES_MEMORY_MODULE_MEMORYAGENT_H_

#include "../include/agent.h"

namespace s21 {

class MemoryAgent : public Agent {
 public:
  MemoryAgent();
  ~MemoryAgent();

  std::vector<Metric> GetMetrics() override;
  std::string GetAgentName() override;
  std::string GetAgentType() override;
};

}  // namespace s21

#endif
