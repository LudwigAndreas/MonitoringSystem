#pragma once

#include <string>
#include <memory>

#include "notifier/telegram/TelegramSender.h"
#include "notifier/email/EmailSender.h"

class CriticalValueHandler {
	EmailSender		email_sender;
	TelegramSender	telegram_sender;

	CriticalValueHandler();

	void sendMessage(FailedMetric fm);
};

using CriticalValueHandlerPtr = std::shared_ptr<CriticalValueHandler>;
