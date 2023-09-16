//
// Created by Ludwig Andreas on 10.09.2023.
//

#ifndef MONITORINGSYSTEM_MOCKAGENT_H_
#define MONITORINGSYSTEM_MOCKAGENT_H_

#include "modules/include/agent.h"
#include "modules/include/metric.h"

class MockAgent : public s21::Agent {
  // Mock the GetMetrics method
  MOCK_METHOD(std::vector<s21::Metric>, GetMetrics, (), (override));

  // Mock the GetAgentName method
  MOCK_METHOD(std::string, GetAgentName, (), (override));

  // Mock the GetAgentType method
  MOCK_METHOD(std::string, GetAgentType, (), (override));
};

#endif
