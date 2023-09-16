//
// Created by Ludwig Andreas on 31.07.2023.
//

#ifndef MONITORINGSYSTEM_SRC_MODULES_TEST_MODULE_AGENT_FACTORY_CC_
#define MONITORINGSYSTEM_SRC_MODULES_TEST_MODULE_AGENT_FACTORY_CC_

#include "modules/include/agent.h"
#include "test_agent.h"

extern "C" {
s21::Agent* get_agent(void) {
    return new s21::TestAgent();
  }
};



#endif //MONITORINGSYSTEM_SRC_MODULES_TEST_MODULE_AGENT_FACTORY_CC_
