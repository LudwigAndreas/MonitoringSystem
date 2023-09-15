//
// Created by Ludwig Andreas on 27.08.2023.
//

#include "AgentExecutor.h"

namespace s21 {

AgentExecutor::AgentExecutor(MetricAnalyzerPtr &logger) {
  logger_ = logger;
}

void AgentExecutor::OnAgentAdded(std::shared_ptr<AgentBundle> &agent) {
  agent_schedulers_.insert({agent, std::make_shared<AgentScheduler>(agent, logger_)});
}

void AgentExecutor::OnAgentRemoved(std::shared_ptr<AgentBundle> &agent) {
  agent_schedulers_.at(agent)->Stop();
  agent_schedulers_.erase(agent);
}

void AgentExecutor::OnAgentUpdated(std::shared_ptr<AgentBundle> &agent) {
  agent_schedulers_.at(agent)->Stop();
  agent_schedulers_.erase(agent);
  agent_schedulers_.insert({agent, std::make_shared<AgentScheduler>(agent, logger_)});
}

std::map<AgentBundlePtr, std::shared_ptr<AgentScheduler>> &AgentExecutor::GetAgentSchedulers() {
  return agent_schedulers_;
}
}