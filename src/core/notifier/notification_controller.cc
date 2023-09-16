//
// Created by Ludwig Andreas on 05.09.2023.
//

#include "notification_controller.h"

namespace s21 {

NotificationController::NotificationController() {

}

NotificationController::~NotificationController() {

}

void NotificationController::AddNotifier(IMessageSenderPtr sender) {
  senders_.emplace_back(sender);
}

void NotificationController::RemoveNotifier(IMessageSenderPtr sender) {
  senders_.erase(std::remove(senders_.begin(), senders_.end(), sender), senders_.end());
}

void NotificationController::OnCriticalValueReached(MetricEvent event) {
  for (const auto& sender: senders_) {
    sender->SendMessage(FailedMetric(event));
  }
}
void NotificationController::OnLineLogged(const std::string &line) {
  (void) line;
}

void NotificationController::OnNewFileOpened(const std::string &log_file) {
  (void) log_file;
}

}

