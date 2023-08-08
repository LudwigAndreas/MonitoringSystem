//
// Created by Ludwig Andreas on 31.07.2023.
//

#ifndef MONITORINGSYSTEM_SRC_MODULES_DUMB_MODULE_DUMBAGENT_H_
#define MONITORINGSYSTEM_SRC_MODULES_DUMB_MODULE_DUMBAGENT_H_

#include "../include/Agent.h"

namespace s21 {

class BonusAgent : public Agent {

 public:

  BonusAgent();
  ~BonusAgent();

  std::vector<Metric> GetMetrics() override;
  std::string GetAgentName() override;
  std::string GetAgentType() override;
};

}

#endif //MONITORINGSYSTEM_SRC_MODULES_DUMB_MODULE_DUMBAGENT_H_
