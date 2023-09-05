//
// Created by Ludwig Andreas on 31.07.2023.
//

#ifndef MONITORINGSYSTEM_SRC_MODULES_DUMB_MODULE_AGENT_FACTORY_CC_
#define MONITORINGSYSTEM_SRC_MODULES_DUMB_MODULE_AGENT_FACTORY_CC_

#include "../include/Agent.h"
#include "CPUAgent.h"

extern "C" {
s21::Agent* get_agent(void) {
    return new s21::CPUAgent();
  }
};



#endif //MONITORINGSYSTEM_SRC_MODULES_DUMB_MODULE_AGENT_FACTORY_CC_
