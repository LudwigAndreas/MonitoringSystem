#pragma once

#include <string>
#include <memory>

#include "notifier/telegram/TelegramSender.h"
#include "notifier/email/EmailSender.h"

namespace s21 {

class CriticalValueHandler {
	EmailSender		email_sender;
	TelegramSender	telegram_sender;

	CriticalValueHandler();

	void SendMessage(FailedMetric fm);
};

using CriticalValueHandlerPtr = std::shared_ptr<CriticalValueHandler>;
}