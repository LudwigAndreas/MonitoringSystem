//
// Created by Ludwig Andreas on 23.08.2023.
//

#include "agent_bundle.h"

#include "Logger.h"

namespace s21 {

AgentBundle::AgentBundle() = default;

AgentBundle::AgentBundle(const AgentPtr &agent,
                         const PropertiesPtr &properties,
                         const std::string &agent_path) {
  name_ = agent->GetAgentName();
  type_ = agent->GetAgentType();
  properties_ = properties;
  agent_path_ = agent_path;
  configured_metrics_ = new std::vector<ConfiguredMetricPtr>();
  for (const Metric &metric: agent->GetMetrics()) {
    configured_metrics_->emplace_back(std::make_shared<ConfiguredMetric>(metric));
  }
  Configure(properties);
}

AgentBundle::~AgentBundle() {
  delete configured_metrics_;
}

void AgentBundle::UpdateBundle(const PropertiesPtr &properties,
                               const std::string &agent_path) {
  agent_path_ = agent_path;
  Configure(properties);
}

std::vector<ConfiguredMetricPtr> *AgentBundle::GetMetrics() const {
  return configured_metrics_;
}

std::string AgentBundle::GetAgentName() const {
  return name_;
}

std::string AgentBundle::GetAgentType() const {
  return type_;
}

const std::string &AgentBundle::GetName() const {
  return name_;
}
void AgentBundle::SetName(const std::string &name) {
  name_ = name;
}
const std::string &AgentBundle::GetType() const {
  return type_;
}
void AgentBundle::SetType(const std::string &type) {
  type_ = type;
}

void AgentBundle::Configure(const PropertiesPtr &properties) {
  ConfigureAgent(properties);
  for (auto &configured_metric: *configured_metrics_) {
    ConfigureMetric(properties, configured_metric);
  }
}

void AgentBundle::ConfigureAgent(const PropertiesPtr &properties) {
  std::string agent_name = properties->GetProperty("agent.name", "");
  std::string agent_type = properties->GetProperty("agent.type", "");
  bool is_enabled = properties->GetProperty("agent.enabled", "false") == "true";

  if (!agent_name.empty()) {
    name_ = agent_name;
    LOG_DEBUG(diagnostic::Logger::getRootLogger(),
             "Agent name is set to " + agent_name);
  } else {
    LOG_WARN(diagnostic::Logger::getRootLogger(), "Agent name is not set");
  }
  if (!agent_type.empty()) {
    type_ = agent_type;
    LOG_DEBUG(diagnostic::Logger::getRootLogger(),
             "Agent type is set to " + agent_type);
  } else {
    LOG_WARN(diagnostic::Logger::getRootLogger(), "Agent type is not set");
  }
  is_enabled_ = is_enabled;
}

void AgentBundle::ConfigureMetric(const PropertiesPtr &properties,
                                  ConfiguredMetricPtr &metric) const {
  std::string metric_name = metric->GetName();

  std::string critical_value = properties->GetProperty(
      "metric." + metric_name + ".critical_value",
      "");

  std::string update_interval = properties->GetProperty(
      "metric." + metric_name + ".update_time",
      "");

  std::string metric_args = properties->GetProperty(
      "metric." + metric_name + ".args",
      "");

  if (!critical_value.empty()) {
    metric->SetCriticalValue(MetricCriticalValue(critical_value));
  } else {
    metric->SetCriticalValue(MetricCriticalValue());
    LOG_WARN(diagnostic::Logger::getRootLogger(), "Critical value for metric "
        + metric_name + " is not set");
  }

  if (!update_interval.empty()) {
    metric->SetUpdateTime(std::stoi(update_interval));
  } else {
    metric->SetUpdateTime(default_update_interval_);
    LOG_WARN(diagnostic::Logger::getRootLogger(), "Update interval for metric "
        + metric_name + " is not set");
  }

  if (!metric_args.empty()) {
    metric->SetArgs(metric_args);
  }
}

std::chrono::system_clock::time_point AgentBundle::GetUptimePoint() const {
  return uptime_point_;
}

void AgentBundle::SetUptimePoint(std::chrono::system_clock::time_point uptime_point) {
  AgentBundle::uptime_point_ = uptime_point;
}

bool AgentBundle::IsEnabled() const {
  return is_enabled_;
}

void AgentBundle::SetIsEnabled(bool is_enabled) {
  AgentBundle::is_enabled_ = is_enabled;
}

time_t AgentBundle::GetLastModified() const {
  return last_modified_;
}

void AgentBundle::SetLastModified(time_t last_modified) {
  last_modified_ = last_modified;
}

void AgentBundle::SaveConfig() {
  properties_->SetProperty("agent.name", name_);
  properties_->SetProperty("agent.type", type_);
  properties_->SetProperty("agent.enabled", is_enabled_ ? "true" : "false");
  for (auto &configured_metric: *configured_metrics_) {
      std::string metric_name = configured_metric->GetName();
      properties_->SetProperty("metric." + metric_name + ".critical_value",
                               configured_metric->GetCriticalValue().ToString());
      properties_->SetProperty("metric." + metric_name + ".update_time",
                               std::to_string(configured_metric->GetUpdateTime()));
      if (!configured_metric->GetArgs().empty())
        properties_->SetProperty("metric." + metric_name + ".args",
                               configured_metric->GetArgs());
  }
  properties_->Save();
}
const std::string &AgentBundle::GetAgentPath() const {
  return agent_path_;
}
void AgentBundle::SetAgentPath(const std::string &agent_path) {
  agent_path_ = agent_path;
}

}
