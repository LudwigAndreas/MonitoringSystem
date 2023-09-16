//
// Created by Ludwig Andreas on 31.07.2023.
//

#ifndef MONITORINGSYSTEM_SRC_MODULES_DUMB_MODULE_AGENT_FACTORY_CC_
#define MONITORINGSYSTEM_SRC_MODULES_DUMB_MODULE_AGENT_FACTORY_CC_

#include "../include/agent.h"
#include "bonus_agent.h"

extern "C" {
s21::Agent *get_agent() { return new s21::BonusAgent(); }
};

#endif
