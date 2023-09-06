#pragma once

#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <set>

#include "notifier/IMessageSender.h"
#include "TelegramUserRepository.h"

#include "tgbot/tgbot.h"

class TelegramSender : public IMessageSender {
 private:
  static const int DEFAULT_USER = -1;
  const std::string __token = "5426071766:AAG3rchPUG-V6gswM3-tPGVDjnG5hVgmBdw";
  TgBot::Bot bot;
  TelegramUserRepository users;
  std::thread polling_thread;
  std::set<std::string> receivers;
  bool is_polling_running = false;

  std::string prepareMessage(FailedMetric fm);

 public:
  TelegramSender();
  virtual ~TelegramSender();

  virtual void sendMessage(FailedMetric fm) override;

  std::set<std::string> getReceivers();
  void addReceiver(std::string username,
                   long chat_id = TelegramSender::DEFAULT_USER);
  void removeReceiver(std::string username);
  void PollingFunction();
};

using TelegramSenderPtr = std::shared_ptr<TelegramSender>;
