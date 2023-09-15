//
// Created by Ludwig Andreas on 10.09.2023.
//

#include "test.h"
#include "agent/agent_bundle.h"
#include "metric/ConfiguredMetric.h"
#include "config/Properties.h"
#include "modules/test_module/TestAgent.h"

class AgentBundleTest : public testing::Test {
 protected:
  void SetUp() override {
    test_agent = std::make_shared<s21::TestAgent>();
    test_properties = std::make_shared<s21::Properties>();
    test_properties->SetProperty("agent.name", "Test Agent");
    test_properties->SetProperty("agent.type", "Test Type");
    test_properties->SetProperty("agent.enabled", "true");
    agent_bundle = std::make_unique<s21::AgentBundle>(test_agent,
                                                      test_properties,
                                                      "test/TestAgent.agent"
                                                      );
  }
  std::shared_ptr<s21::TestAgent> test_agent;
  std::shared_ptr<s21::Properties> test_properties;
  std::unique_ptr<s21::AgentBundle> agent_bundle;
};

TEST_F(AgentBundleTest, FromBundleConfig) {
  EXPECT_EQ(agent_bundle->GetAgentName(), "Test Agent");
  EXPECT_EQ(agent_bundle->GetAgentType(), "Test Type");
  EXPECT_EQ(agent_bundle->GetAgentPath(), "test/TestAgent.agent");
  EXPECT_EQ(agent_bundle->IsEnabled(), true);
  EXPECT_EQ(agent_bundle->GetMetrics()->size(), test_agent->GetMetrics().size());
  EXPECT_EQ(agent_bundle->GetMetrics()->at(0)->GetName(), "test_thing");
  EXPECT_EQ(agent_bundle->GetMetrics()->at(1)->GetName(), "test_thing2");
  EXPECT_EQ(agent_bundle->GetMetrics()->at(2)->GetName(), "test_thing3");
}

TEST_F(AgentBundleTest, Updating) {
  test_properties->SetProperty("agent.name", "New Test Agent");
  test_properties->SetProperty("agent.type", "New Test Type");
  test_properties->SetProperty("agent.enabled", "false");
  time_t created = agent_bundle->GetLastModified();
  agent_bundle->UpdateBundle(test_properties, "test/NewTestAgent.agent");
  EXPECT_GE(agent_bundle->GetLastModified(), created);
  EXPECT_EQ(agent_bundle->GetAgentName(), "New Test Agent");
  EXPECT_EQ(agent_bundle->GetAgentType(), "New Test Type");
  EXPECT_EQ(agent_bundle->IsEnabled(), false);
  EXPECT_EQ(agent_bundle->GetAgentPath(), "test/NewTestAgent.agent");
  EXPECT_EQ(agent_bundle->GetMetrics()->size(), test_agent->GetMetrics().size());
  EXPECT_EQ(agent_bundle->GetMetrics()->at(0)->GetName(), "test_thing");
  EXPECT_EQ(agent_bundle->GetMetrics()->at(1)->GetName(), "test_thing2");
  EXPECT_EQ(agent_bundle->GetMetrics()->at(2)->GetName(), "test_thing3");
}


TEST(EmptyAgentBundleTest, EmptyConfiguration) {
  auto test_agent = std::make_shared<s21::TestAgent>();
  auto test_properties = std::make_shared<s21::Properties>();
  auto agent_bundle = std::make_unique<s21::AgentBundle>(test_agent,
                                                         test_properties,
                                                         "test/TestAgent.agent"
  );
  EXPECT_EQ(agent_bundle->GetAgentName(), "test agent name");
  EXPECT_EQ(agent_bundle->GetAgentType(), "test agent type");
  EXPECT_EQ(agent_bundle->IsEnabled(), true);
  EXPECT_EQ(agent_bundle->GetAgentPath(), "test/TestAgent.agent");
  EXPECT_EQ(agent_bundle->GetMetrics()->size(), test_agent->GetMetrics().size());
  EXPECT_EQ(agent_bundle->GetMetrics()->at(0)->GetName(), "test_thing");
  EXPECT_EQ(agent_bundle->GetMetrics()->at(1)->GetName(), "test_thing2");
  EXPECT_EQ(agent_bundle->GetMetrics()->at(2)->GetName(), "test_thing3");
}

TEST(EmptyAgentBundleTest, SetAndGet) {
  auto test_agent = std::make_shared<s21::TestAgent>();
  auto test_properties = std::make_shared<s21::Properties>();
  auto agent_bundle = std::make_unique<s21::AgentBundle>(test_agent,
                                                         test_properties,
                                                         "test/TestAgent.agent"
  );
  agent_bundle->SetName("New Test Agent");
  agent_bundle->SetType("New Test Type");
  agent_bundle->SetIsEnabled(false);
  agent_bundle->SetAgentPath("test/NewTestAgent.agent");
  agent_bundle->SetLastModified(123456789);
  EXPECT_EQ(agent_bundle->GetAgentName(), "New Test Agent");
  EXPECT_EQ(agent_bundle->GetAgentType(), "New Test Type");
  EXPECT_EQ(agent_bundle->IsEnabled(), false);
  EXPECT_EQ(agent_bundle->GetAgentPath(), "test/NewTestAgent.agent");
  EXPECT_EQ(agent_bundle->GetMetrics()->size(), test_agent->GetMetrics().size());
  EXPECT_EQ(agent_bundle->GetLastModified(), 123456789);
}

TEST(EmptyAgentBundleTest, EmptyConstructor) {
  auto agent_bundle = s21::AgentBundle();
  EXPECT_EQ(agent_bundle.GetAgentName(), "");
  EXPECT_EQ(agent_bundle.GetAgentType(), "");
  EXPECT_EQ(agent_bundle.IsEnabled(), false);
  EXPECT_EQ(agent_bundle.GetAgentPath(), "");
  EXPECT_EQ(agent_bundle.GetMetrics()->size(), 0);
}

TEST_F(AgentBundleTest, Config) {
  agent_bundle->SetName("New Test Agent");
  agent_bundle->SetType("New Test Type");
  agent_bundle->SetIsEnabled(false);
  agent_bundle->SetAgentPath("test/NewTestAgent.agent");
  agent_bundle->SetLastModified(123456789);
  for (auto &configured_metric: *agent_bundle->GetMetrics()) {
    configured_metric->SetArgs("New Args");
    configured_metric->SetCriticalValue(s21::MetricCriticalValue(">=123"));
    configured_metric->SetUpdateTime(123456789);
  }
  agent_bundle->SaveConfig();
  EXPECT_EQ(test_properties->GetProperty("agent.name"), "New Test Agent");
  EXPECT_EQ(test_properties->GetProperty("agent.type"), "New Test Type");
  EXPECT_EQ(test_properties->GetProperty("agent.enabled"), "false");
  for (auto &configured_metric: *agent_bundle->GetMetrics()) {
    EXPECT_EQ(test_properties->GetProperty("metric." + configured_metric->GetName() + ".args"),
              configured_metric->GetArgs());
    EXPECT_EQ(test_properties->GetProperty("metric." + configured_metric->GetName() + ".critical_value"),
              configured_metric->GetCriticalValue().ToString());
    EXPECT_EQ(test_properties->GetProperty("metric." + configured_metric->GetName() + ".update_time"),
              std::to_string(configured_metric->GetUpdateTime()));
  }
}