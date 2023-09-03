//
// Created by Ludwig Andreas on 02.09.2023.
//

#ifndef MONITORINGSYSTEM_MAINCONTROLLER_H_
#define MONITORINGSYSTEM_MAINCONTROLLER_H_

#include <vector>
#include <agent/AgentManager.h>

namespace s21 {

class MainController {
 public:
  std::vector<s21::AgentBundlePtr>* GetAgentsList();




 private:
  std::vector<s21::AgentBundlePtr>* agents_list;
};

}

#endif //MONITORINGSYSTEM_MAINCONTROLLER_H_
