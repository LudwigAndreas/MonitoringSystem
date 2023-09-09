#include "EmailSender.h"

#include <iostream>

EmailSender::EmailSender() {}

std::string EmailSender::prepareSubject(FailedMetric fm) {
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
  if (!receivers.empty()) {
    Email email(
      EmailAddress(__email, "kdancybot"),
      std::vector<EmailAddress>(std::next(receivers.begin()), receivers.end()),
      prepareSubject(fm),
      PrepareMessage(fm)
    );
    email.SendMessage(
      __server,
      __email,
      __password
    );
  }
}

std::set<std::string> EmailSender::GetRecievers() {
  return receivers;
}

void EmailSender::AddReceiver(std::string username) {
  receivers.insert(username);
}

void EmailSender::removeReceiver(std::string username) {
  receivers.erase(username);
}