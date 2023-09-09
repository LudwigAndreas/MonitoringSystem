//
// Created by Ludwig Andreas on 23.08.2023.
//

#ifndef MONITORINGSYSTEM_SRC_CORE_AGENT_BUNDLE_H_
#define MONITORINGSYSTEM_SRC_CORE_AGENT_BUNDLE_H_

#include <filesystem>

#include "metric/ConfiguredMetric.h"
#include "config/Properties.h"
#include "../../modules/include/Agent.h"

namespace s21 {

typedef std::shared_ptr<Agent> AgentPtr;
typedef std::shared_ptr<Properties> PropertiesPtr;
typedef std::shared_ptr<ConfiguredMetric> ConfiguredMetricPtr;
typedef void *DynamicLib;


/**
 * AgentBundle is a class that bundles an Agent with its Properties.
 * Not a successor to Agent, but has the same interface
 * (but works with ConfiguredMetric).
 *
 * @see Agent
 * @see ConfiguredMetric
 * @see Properties
 *
 * */
class AgentBundle {
 public:

  /**
   * Constructor for AgentBundle.
   * After reading from the library, the Agent is converted to an AgentBundle
   * and loaded with Properties. Then only AgentBundle is used in the program.
   *
   * @param agent The agent to be bundled.
   *
   * @param properties The properties of the agent.
   * */
  AgentBundle(const AgentPtr& agent, const PropertiesPtr& properties, const std::string &agent_path);

  ~AgentBundle();

  /**
   * Returns configured metrics. Configuration loaded from Properties.
   *
   * @return std::vector<ConfiguredMetricPtr> of configured metrics
   *
   * @see Agent::GetMetrics()
   *
   * */
  std::vector<ConfiguredMetricPtr>* GetMetrics() const;

  /**
   * Returns the name of the agent.
   *
   * @return std::string name of the agent
   *
   * @see Agent::GetAgentName()
   *
   * */
  std::string GetAgentName() const;

  /**
   * Returns the type of the agent.
   *
   * @return std::string type of the agent
   *
   * @see Agent::GetAgentType()
   * */
  std::string GetAgentType() const;

  std::chrono::system_clock::time_point GetUptimePoint() const;

  void SetUptimePoint(std::chrono::system_clock::time_point uptime_point);

  bool IsEnabled() const;

  void SetIsEnabled(bool is_enabled);

  /**
   * Updates the agent's properties.
   *
   * @param properties The properties to be set.
   *
   * @see Properties
   * */
  void Configure(const PropertiesPtr& properties);



 protected:

  /**
   * Constructor for AgentBundle.
   * */
  AgentBundle();

  void ConfigureAgent(const PropertiesPtr& properties);

  void ConfigureMetric(const PropertiesPtr& properties, ConfiguredMetricPtr &metric) const;

  std::string name_;
  std::string type_;
  std::vector<ConfiguredMetricPtr>* configured_metrics_;
  size_t default_update_interval_ = 60;
  std::chrono::system_clock::time_point uptime_point_;
  std::string agent_path_;
  bool is_enabled_ = true;
};

}

#endif //MONITORINGSYSTEM_SRC_CORE_AGENT_BUNDLE_H_
