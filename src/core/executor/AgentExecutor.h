//
// Created by Ludwig Andreas on 27.08.2023.
//

#ifndef MONITORINGSYSTEM_AGENTEXECUTOR_H_
#define MONITORINGSYSTEM_AGENTEXECUTOR_H_

#include <queue>

#include "analyzer/MetricAnalyzer.h"
#include "agent/IAgentSubscriber.h"
#include "metric/MetricEvent.h"
#include "AgentScheduler.h"

namespace s21 {

typedef std::shared_ptr<MetricAnalyzer> MetricAnalyzerPtr;

class AgentExecutor : public IAgentSubscriber {

 public:

  AgentExecutor(MetricAnalyzerPtr &logger);

  void OnAgentAdded(std::shared_ptr<AgentBundle> &agent) override;

  void OnAgentRemoved(std::shared_ptr<AgentBundle> &agent) override;

  void OnAgentUpdated(std::shared_ptr<AgentBundle> &agent) override;

 private:

  MetricAnalyzerPtr logger_;
  std::mutex metric_queue_mutex_;
  std::queue<MetricEvent> metric_queue_;
  std::map<AgentBundlePtr , std::shared_ptr<AgentScheduler>> agent_schedulers_;
};

}

#endif //MONITORINGSYSTEM_AGENTEXECUTOR_H_
