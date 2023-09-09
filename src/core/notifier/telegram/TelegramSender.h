#pragma once

#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <set>

#include "config/Properties.h"
#include "notifier/IMessageSender.h"
#include "TelegramUserRepository.h"

#include "tgbot/tgbot.h"

namespace s21 {

class TelegramSender : public IMessageSender {
 private:
  static const int        DEFAULT_USER        = -1;
  const std::string       config_file         = "config/telegram.properties";
  const std::string       __token             = "5426071766:AAG3rchPUG-V6gswM3-tPGVDjnG5hVgmBdw";
  Properties              properties;
  bool                    is_polling_running  = false;
  TgBot::Bot              bot;
  TelegramUserRepository  users;
  std::thread             polling_thread;
  std::set<std::string>   receivers;

  void        ConfigPolling();
  void        StartPolling();
  void        StopPolling();
  void        InitializeReceivers(std::string receivers);
  std::string PrepareMessage(FailedMetric fm);

 public:
  TelegramSender(std::string receivers = "");
  virtual ~TelegramSender();

  virtual void SendMessage(FailedMetric fm) override;

  std::set<std::string> GetRecievers();
  void AddReceiver(std::string username,
                   long chat_id = TelegramSender::DEFAULT_USER);
  void RemoveReceiver(std::string username);
  void PollingFunction();
};

using TelegramSenderPtr = std::shared_ptr<TelegramSender>;

}