#pragma once

#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <set>

#include <string.h>
#include <curl/curl.h>

#include "notifier/IMessageSender.h"

#define __email "andrew02541632@gmail.com"
#define __password "mzgonnorlsctjzfn"
#define __server "smtp://smtp.gmail.com:587"

struct upload_status {
  size_t bytes_read;
};

class EmailSender : public IMessageSender {
 private:
  std::set<std::string> receivers;
  std::string message;

  size_t payloadSource(char *ptr, size_t size, size_t nmemb, void *userp);
  std::string prepareMessage(FailedMetric fm);

 public:
  EmailSender();
  virtual ~EmailSender() = default;

  virtual void sendMessage(FailedMetric fm);

  std::set<std::string> getReceivers();
  void addReceiver(std::string username);
  void removeReceiver(std::string username);
};

using EmailSenderPtr = std::shared_ptr<EmailSender>;