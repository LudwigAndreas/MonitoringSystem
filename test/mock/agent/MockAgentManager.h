//
// Created by Ludwig Andreas on 10.09.2023.
//

#ifndef MONITORINGSYSTEM_MOCKAGENTMANAGER_H_
#define MONITORINGSYSTEM_MOCKAGENTMANAGER_H_

#include "agent/AgentManager.h"

class MockAgentManager : public s21::AgentManager {

  void StartMonitoring();

  void StopMonitoring();

  bool IsMonitoring();

  std::map<std::string, AgentBundlePtr>* GetAgentList();

  void Subscribe(IAgentSubscriber* subscriber);

  void Unsubscribe(IAgentSubscriber* subscriber);

  void DeleteAgent(std::shared_ptr<AgentBundle> &agent);
};

#endif //MONITORINGSYSTEM_MOCKAGENTMANAGER_H_
