//
// Created by Ludwig Andreas on 10.09.2023.
//

#ifndef MONITORINGSYSTEM_MOCKAGENTSUBSCRIBER_H_
#define MONITORINGSYSTEM_MOCKAGENTSUBSCRIBER_H_

#include "test.h"

// Mock class for IAgentSubscriber
class MockAgentSubscriber: public s21::IAgentSubscriber {
 public:
  MOCK_METHOD1(OnAgentAdded, void(std::shared_ptr<s21::AgentBundle> &agent));
  MOCK_METHOD1(OnAgentRemoved, void(std::shared_ptr<s21::AgentBundle> &agent));
  MOCK_METHOD1(OnAgentUpdated, void(std::shared_ptr<s21::AgentBundle> &agent));
};

#endif //MONITORINGSYSTEM_MOCKAGENTSUBSCRIBER_H_
