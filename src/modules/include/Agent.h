//
// Created by Ludwig Andreas on 30.07.2023.
//

#ifndef MONITORINGSYSTEM_SRC_MODULES_DUMB_MODULE_AGENT_H_
#define MONITORINGSYSTEM_SRC_MODULES_DUMB_MODULE_AGENT_H_

#include <vector>
#include <string>

#include "Metric.h"

namespace s21 {
class Agent {
 public:

  virtual std::vector<Metric> GetMetrics() = 0;

  virtual std::string GetAgentName() = 0;

  virtual std::string GetAgentType() = 0;

};
}

#endif //MONITORINGSYSTEM_SRC_MODULES_DUMB_MODULE_AGENT_H_
