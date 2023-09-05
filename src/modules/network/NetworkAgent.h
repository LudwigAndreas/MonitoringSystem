
#ifndef MONITORINGSYSTEM_SRC_MODULES_NETWORK_MODULE_NETWORKAGENT_H_
#define MONITORINGSYSTEM_SRC_MODULES_NETWORK_MODULE_NETWORKAGENT_H_

#include "../include/Agent.h"

namespace s21 {

class NetworkAgent : public Agent {

 public:

  NetworkAgent();
  ~NetworkAgent();

  std::vector<Metric> GetMetrics() override;
  std::string GetAgentName() override;
  std::string GetAgentType() override;
};

}

#endif //MONITORINGSYSTEM_SRC_MODULES_NETWORK_MODULE_NETWORKAGENT_H_