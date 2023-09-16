#ifndef TELEGRAMBOT_H
#define TELEGRAMBOT_H

#include "i_telegram_bot.h"
#include "logger.h"
#include "tgbot/Bot.h"
#include "tgbot/net/TgLongPoll.h"

namespace s21 {

class TelegramBot : public ITelegramBot {
  TgBot::Bot bot;
  TgBot::TgLongPoll poll;

 public:
  TelegramBot(std::string token);
  virtual ~TelegramBot() = default;
  virtual void OnStart(
      const std::function<void(const TgBot::Message::Ptr)> &) override;
  virtual void SendMessage(const std::string &chat_id,
                           const std::string &message) override;
  virtual void LongPoll() override;
};

}  // namespace s21

#endif