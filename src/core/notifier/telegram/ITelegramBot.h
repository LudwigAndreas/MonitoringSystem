#ifndef ITELEGRAMBOT_H
#define ITELEGRAMBOT_H

#include <functional>

#include "tgbot/types/Message.h"

#include "notifier/FailedMetric.h"

namespace s21 {

class ITelegramBot {
public:
	virtual void OnStart(const std::function<void(const TgBot::Message::Ptr)> &) = 0;
	virtual void SendMessage(const std::string& chat_id, const std::string& message) = 0;
	virtual void LongPoll() = 0;
};

}

#endif