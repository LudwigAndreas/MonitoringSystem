#pragma once

#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <set>

#include <string.h>
#include <curl/curl.h>

#include "email.h"
#include "notifier/i_message_sender.h"

namespace s21 {


class EmailSender : public IMessageSender {
 private:
  std::set<std::string> receivers;
  std::string           email_;
  std::string           password_;
  std::string           server_;
  bool                  enabled_;

  std::string PrepareSubject(FailedMetric fm);
  std::string PrepareMessage(FailedMetric fm);

 public:
  EmailSender(std::string email, std::string password, std::string server);
  virtual ~EmailSender() = default;

  virtual void SendMessage(FailedMetric fm) override;
  virtual void SetEnabled(bool enabled) override;

  std::set<std::string> GetRecievers();
  void AddReceiver(std::string username);
  void RemoveReceiver(std::string username);
};

using EmailSenderPtr = std::shared_ptr<EmailSender>;

}