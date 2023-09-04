//
// Created by Ludwig Andreas on 23.08.2023.
//

#include "agent_bundle.h"

#include "Logger.h"

namespace s21 {

AgentBundle::AgentBundle() = default;

AgentBundle::AgentBundle(const AgentPtr& agent, const PropertiesPtr& properties) {
  name_ = agent->GetAgentName();
  type_ = agent->GetAgentType();
  configured_metrics_ = new std::vector<ConfiguredMetricPtr>();
  for (const Metric &metric: agent->GetMetrics()) {
    configured_metrics_->emplace_back(std::make_shared<ConfiguredMetric>(metric));
  }
  Configure(properties);
}

AgentBundle::~AgentBundle() {
  delete configured_metrics_;
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

void AgentBundle::Configure(const PropertiesPtr& properties) {
  for (auto &configured_metric: *configured_metrics_) {
    ConfigureMetric(properties, configured_metric);
  }
}

void AgentBundle::ConfigureMetric(const PropertiesPtr& properties,
                                  ConfiguredMetricPtr &metric) const {
  std::string metric_name = metric->GetName();
  std::string critical_value = properties->GetProperty(
      "metric." + metric_name + ".critical_value",
      "");
  if (!critical_value.empty()) {
    metric->SetCriticalValue(MetricCriticalValue(critical_value));
  } else {
    metric->SetCriticalValue(MetricCriticalValue());
    LOG_WARN(diagnostic::Logger::getRootLogger(), "Critical value for metric "
        + metric_name + " is not set");
  }
  std::string update_interval = properties->GetProperty(
      "metric." + metric_name + ".update_time",
      "");
  if (!update_interval.empty()) {
    metric->SetUpdateTime(std::stoi(update_interval));
  } else {
    metric->SetUpdateTime(default_update_interval_);
    LOG_WARN(diagnostic::Logger::getRootLogger(), "Update interval for metric "
        + metric_name + " is not set");
  }
    std::string metric_args = properties->GetProperty(
        "metric." + metric_name + ".args",
        "");
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

}
