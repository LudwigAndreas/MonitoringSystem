//
// Created by Ludwig Andreas on 31.07.2023.
//

#ifndef MONITORINGSYSTEM_SRC_MODULES_TEST_MODULE_TESTAGENT_H_
#define MONITORINGSYSTEM_SRC_MODULES_TEST_MODULE_TESTAGENT_H_

#include "modules/include/Agent.h"

namespace s21 {

class TestAgent : public Agent {

 public:

  TestAgent();
  ~TestAgent();

  std::vector<Metric> GetMetrics() override;
  std::string GetAgentName() override;
  std::string GetAgentType() override;
};

}

#endif //MONITORINGSYSTEM_SRC_MODULES_TEST_MODULE_TESTAGENT_H_
