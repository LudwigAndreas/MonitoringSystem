#include "EmailSender.h"

#include <iostream>

namespace s21 {

EmailSender::EmailSender(std::string email, std::string password, std::string server) {
  this->email_ = email;
  this->password_ = password;
  this->server_ = server;
  this->enabled_ = true;
  this->receivers = std::set<std::string>();
}

std::string EmailSender::PrepareSubject(FailedMetric fm) {
  std::ostringstream ss;
  ss  << "Failed metric "
    << "\"" << fm.metric_name << "\""
    << " on "
    << "[" << this->hostname << "]";
  return ss.str();
}

std::string EmailSender::PrepareMessage(FailedMetric fm) {
  std::ostringstream ss;
  ss  << "Metric: "         << "\"" << fm.metric_name << "\""     << "\n"
    << "Hostname: "       << "[" << this->hostname << "]"       << "\n"
    << "Date: "           << fm.date                            /*<< "\n"*/
    << "Value: "          << fm.value                           << "\n"
    << "Critical value: " << "\"" << fm.critical_value << "\""  << ".";
  return ss.str();
}

void EmailSender::SendMessage(FailedMetric fm) {
  if (enabled_) {
    if (!receivers.empty()) {
      Email email(
        EmailAddress(email_, "kdancybot"),
        std::vector<EmailAddress>(std::next(receivers.begin()), receivers.end()),
        PrepareSubject(fm),
        PrepareMessage(fm)
      );
      email.SendMessage(
        server_,
        email_,
        password_
      );
    }
  }
}

void EmailSender::SetEnabled(bool enabled) {
  enabled_ = enabled;
}

std::set<std::string> EmailSender::GetRecievers() {
  return receivers;
}

void EmailSender::AddReceiver(std::string username) {
  receivers.insert(username);
}

void EmailSender::RemoveReceiver(std::string username) {
  receivers.erase(username);
}

}