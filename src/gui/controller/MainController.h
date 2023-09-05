//
// Created by Ludwig Andreas on 02.09.2023.
//

#ifndef MONITORINGSYSTEM_MAINCONTROLLER_H_
#define MONITORINGSYSTEM_MAINCONTROLLER_H_

#include "metric/MetricEvent.h"
#include "core/Core.h"
#include "agent/IAgentSubscriber.h"
#include "metric/IMetricSubscriber.h"
#include "../view/mainwindow.h"

namespace s21 {

class MainController : public IAgentSubscriber, public IMetricSubscriber {

  public:

  MainController(std::shared_ptr<MainWindow> &window, std::shared_ptr<s21::monitor::Core> &core);
  ~MainController();

  void OnAgentAdded(std::shared_ptr<AgentBundle> &agent) override;
  void OnAgentRemoved(std::shared_ptr<AgentBundle> &agent) override;
  void OnAgentUpdated(std::shared_ptr<AgentBundle> &agent) override;
  void OnCriticalValueReached(MetricEvent event) override;
  void OnLineLogged(const std::string &line) override;
  void OnNewFileOpened(const std::string &log_file) override;

  void Quit();
  void Close();

  private:

  std::shared_ptr<MainWindow> &mainwindow_;
  std::shared_ptr<s21::monitor::Core> &core_;
};

}

#endif //MONITORINGSYSTEM_MAINCONTROLLER_H_
