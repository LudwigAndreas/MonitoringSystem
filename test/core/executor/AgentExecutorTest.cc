//
// Created by Ludwig Andreas on 14.09.2023.
//

#include "test.h"
#include "executor/AgentExecutor.h"
#include "mock/metric/MockMetricAnalyzer.h"
#include "Logger.h"

class AgentExecutorTest : public ::testing::Test {
 protected:
  void SetUp() override {
//    auto logger = s21::diagnostic::Logger::getLogger("AgentExecutorTest");
//    std::shared_ptr<s21::MetricAnalyzer> analyzer = std::make_shared<MockMetricAnalyzer>(logger);
//    agent_executor_ = std::make_shared<s21::AgentExecutor>(logger);
  }

  void TearDown() override {
  }

//  std::shared_ptr<s21::monitor::AgentExecutor> agent_executor_;
};

