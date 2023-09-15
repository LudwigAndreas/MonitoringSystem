#pragma once

#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <set>
#include <thread>

#include "config/Properties.h"
#include "notifier/IMessageSender.h"
#include "TelegramUserRepository.h"
#include "TelegramBot.h"


namespace s21 {

class TelegramSender : public IMessageSender {
 private:
  static const std::string      DEFAULT_USER;
  Properties                    properties;
  bool                          is_polling_running  = false;
  std::shared_ptr<ITelegramBot> bot;
  std::shared_ptr<IUserRepository>        users;
  std::shared_ptr<std::thread>  polling_thread;
  std::set<std::string>         receivers;
  bool                          enabled_;

  void        ConfigPolling();
  void        StartPolling();
  void        StopPolling();

  bool        PollingCheck();
  void        PollingFunctionCheck();
  void        PollingFunction();
  
  void        InitializeRepository();
  void        UpdateRepository(const std::string &receiver);
  void        InitializeReceivers(std::string receivers);
  
  std::string PrepareMessage(FailedMetric fm);

 public:
  TelegramSender(
    const std::shared_ptr<ITelegramBot> &bot, 
    const std::shared_ptr<IUserRepository> &users, 
    std::string receivers = "");
  virtual ~TelegramSender();

  virtual void SendMessage(FailedMetric fm) override;
  virtual void SetEnabled(bool enabled) override;

  std::set<std::string> GetRecievers();
  void AddReceiver(std::string username,
                   std::string chat_id = "");
  void RemoveReceiver(std::string username);
};

using TelegramSenderPtr = std::shared_ptr<TelegramSender>;

}