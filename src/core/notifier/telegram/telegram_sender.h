#pragma once

#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "config/properties.h"
#include "logger.h"
#include "notifier/i_message_sender.h"
#include "telegram_bot.h"
#include "telegram_user_repository.h"

namespace s21 {

class TelegramSender : public IMessageSender {
 private:
  bool is_polling_running = false;
  std::shared_ptr<ITelegramBot> bot;
  std::shared_ptr<IUserRepository> users;
  std::shared_ptr<std::thread> polling_thread;
  std::set<std::string> receivers;
  bool enabled_;

  void ConfigPolling();
  void StartPolling();
  void StopPolling();

  bool PollingCheck();
  void PollingFunctionCheck();
  void PollingFunction();

  void InitializeReceivers(std::string receivers);

  std::string PrepareMessage(FailedMetric fm);

 public:
  TelegramSender(const std::shared_ptr<ITelegramBot> &bot,
                 const std::shared_ptr<IUserRepository> &users,
                 std::string receivers = "");
  virtual ~TelegramSender();

  virtual void SendMessage(FailedMetric fm) override;
  virtual void SetEnabled(bool enabled) override;

  std::set<std::string> GetRecievers();
  void AddReceiver(std::string username, std::string chat_id = EMPTY_CHAT_ID);
  void RemoveReceiver(std::string username);
  void RemoveReceivers();
};

using TelegramSenderPtr = std::shared_ptr<TelegramSender>;

}  // namespace s21