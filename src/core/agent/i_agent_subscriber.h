//
// Created by Ludwig Andreas on 26.08.2023.
//

#ifndef MONITORINGSYSTEM_IAGENTSUBSCRIBER_H_
#define MONITORINGSYSTEM_IAGENTSUBSCRIBER_H_

#include "agent/agent_bundle.h"

namespace s21 {

class IAgentSubscriber {
 public:
  virtual void OnAgentAdded(std::shared_ptr<AgentBundle> &agent) = 0;
  virtual void OnAgentRemoved(std::shared_ptr<AgentBundle> &agent) = 0;
  virtual void OnAgentUpdated(std::shared_ptr<AgentBundle> &agent) = 0;

  virtual ~IAgentSubscriber() = default;
};

}  // namespace s21

#endif
