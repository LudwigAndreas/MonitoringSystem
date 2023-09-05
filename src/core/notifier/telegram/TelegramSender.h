#pragma once

#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <set>

#include "notifier/IMessageSender.h"
#include "TelegramUserRepository.h"

#include "tgbot/tgbot.h"

#define __token "5426071766:AAG3rchPUG-V6gswM3-tPGVDjnG5hVgmBdw"

class TelegramSender : public IMessageSender {
 private:
  static const int DEFAULT_USER = -1;
  TgBot::Bot bot;
  TelegramUserRepository users;
  std::set<std::string> receivers;

  std::string prepareMessage(FailedMetric fm);

 public:
  TelegramSender();
  virtual ~TelegramSender() = default;

  virtual void sendMessage(FailedMetric fm) override;

  std::set<std::string> getReceivers();
  void addReceiver(std::string username,
                   int chat_id = TelegramSender::DEFAULT_USER);
  void removeReceiver(std::string username);
};

using TelegramSenderPtr = std::shared_ptr<TelegramSender>;
