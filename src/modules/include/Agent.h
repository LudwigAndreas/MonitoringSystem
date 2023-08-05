//
// Created by Ludwig Andreas on 30.07.2023.
//

#ifndef MONITORINGSYSTEM_SRC_MODULES_DUMB_MODULE_AGENT_H_
#define MONITORINGSYSTEM_SRC_MODULES_DUMB_MODULE_AGENT_H_

#include <vector>
#include <string>

#include "Metric.h"

namespace s21 {

/**
 * The Agent class is an interface class that provides functionality
 * for using the agent's successors and all of its metrics.
 *
 * Each agent should provide access to the name, type, and list of Metrics
 *
 */
class Agent {
 public:

  virtual ~Agent() = default;

/**
 * The method should provide a list of objects of the metric class.
 *
 * @note The method is required to be implemented.
 *
 * @return a list of metric objects
 */
  virtual std::vector<Metric> GetMetrics() = 0;

/**
 * The method should provide Agent's name.
 * The name can be changed during program execution.
 *
 * @note The method is required to be implemented.
 *
 * @return the name of the Agent
 */
  virtual std::string GetAgentName() = 0;

/**
 * The method should provide Agent's type.
 * The type can be changed during program execution.
 *
 * @note The method is required to be implemented.
 *
 * @return the type of the Agent
 */
  virtual std::string GetAgentType() = 0;
};
}

#endif //MONITORINGSYSTEM_SRC_MODULES_DUMB_MODULE_AGENT_H_
