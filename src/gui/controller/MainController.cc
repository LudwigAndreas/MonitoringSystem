//
// Created by Ludwig Andreas on 02.09.2023.
//

#include "MainController.h"

#include <utility>

namespace s21 {

MainController::MainController(std::shared_ptr<MainWindow> &window, std::shared_ptr<s21::monitor::Core> &core)
    : mainwindow_(window), core_(core) {}

MainController::~MainController() {

}

void MainController::OnAgentAdded(std::shared_ptr<AgentBundle> &agent) {
 mainwindow_->AddAgent(agent);
}

void MainController::OnAgentRemoved(std::shared_ptr<AgentBundle> &agent) {
  mainwindow_->RemoveAgent(agent);
}

void MainController::OnAgentUpdated(std::shared_ptr<AgentBundle> &agent) {
  mainwindow_->UpdateAgent(agent);
}

void MainController::OnCriticalValueReached(MetricEvent event) {
  mainwindow_->OnCriticalValueReached(event);
}

void MainController::OnLineLogged(const std::string &line) {
  mainwindow_->InsertLogLine(line);
}

void MainController::OnNewFileOpened(const std::string &log_file) {
  mainwindow_->SetLogFile(log_file);
}

void MainController::Quit() {
  core_->DisableMonitoring();
}

void MainController::Close() {

}

}