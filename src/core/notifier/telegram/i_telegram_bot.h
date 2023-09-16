#ifndef ITELEGRAMBOT_H
#define ITELEGRAMBOT_H

#include <functional>

#include "notifier/failed_metric.h"
#include "tgbot/types/Message.h"

namespace s21 {

class ITelegramBot {
public:
	virtual void OnStart(const std::function<void(const TgBot::Message::Ptr)> &) = 0;
	virtual void SendMessage(const std::string& chat_id, const std::string& message) = 0;
	virtual bool LongPoll() = 0;
};

}  // namespace s21

#endif