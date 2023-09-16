//
// Created by Ludwig Andreas on 02.09.2023.
//

#include "main_controller.h"

#include <utility>

namespace s21 {

MainController::MainController(std::shared_ptr<MainWindow> &window,
                               std::shared_ptr<s21::monitor::Core> &core)
    : mainwindow_(window), core_(core) {}

MainController::~MainController() {}

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

void MainController::DeleteAgent(std::shared_ptr<AgentBundle> &agent) {
  core_->DeleteAgent(agent);
}

void MainController::SaveConfiguration(std::shared_ptr<AgentBundle> &agent) {
  agent->SaveConfig();
}

void MainController::Quit() { core_->DisableMonitoring(); }

void MainController::SetTelegram(const TelegramSenderPtr &telegram) {
  telegram_ = telegram;
}

void MainController::SetEmail(const EmailSenderPtr &email) { email_ = email; }

void MainController::SetTelegram(const std::string &sender, bool is_enabled) {
  std::set<std::string> recievers = telegram_->GetRecievers();
  if (recievers.empty()) {
    telegram_->AddReceiver(sender);
    return;
  }
  for (auto &reciever : recievers) {
    telegram_->RemoveReceiver(reciever);
  }
  telegram_->AddReceiver(sender);
  telegram_->SetEnabled(is_enabled);
}

void MainController::SetEmail(const std::string &sender, bool is_enabled) {
  std::set<std::string> recievers = email_->GetRecievers();
  if (recievers.empty()) {
    email_->AddReceiver(sender);
    return;
  }
  for (auto &reciever : recievers) {
    email_->RemoveReceiver(reciever);
  }
  email_->AddReceiver(sender);
  email_->SetEnabled(is_enabled);
}

std::string MainController::GetTelegramSender() {
  std::set<std::string> recievers = telegram_->GetRecievers();
  return recievers.empty() ? std::string() : *recievers.begin();
}

std::string MainController::GetEmailSender() {
  std::set<std::string> recievers = email_->GetRecievers();
  return recievers.empty() ? std::string() : *recievers.begin();
}

}  // namespace s21