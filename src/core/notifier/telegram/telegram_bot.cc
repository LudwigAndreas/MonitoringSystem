#include "telegram_bot.h"

namespace s21 {

TelegramBot::TelegramBot(std::string token) : bot(token), poll(bot), is_valid(true) {}

void TelegramBot::OnStart(const std::function<void(const TgBot::Message::Ptr)> &action) {
	bot.getEvents().onCommand("start", action);
}

void TelegramBot::SendMessage(const std::string& chat_id, const std::string& message) {
	if (is_valid) {
		try {
			bot.getApi().sendMessage(chat_id, message);
		} catch (TgBot::TgException &e) {
			LOG_WARN(diagnostic::Logger::getRootLogger(), "Telegram Bot failed to send a message: " << e.what() << ". Check bot's credentials.")
			is_valid = false;
		}
	}
}

bool TelegramBot::LongPoll() {
	if (is_valid) {
		try {
			poll.start();
		} catch (TgBot::TgException &e) {
			LOG_WARN(diagnostic::Logger::getRootLogger(), "Telegram Bot failed to poll: " << e.what() << ". Check bot's credentials.")
			is_valid = false;
		}
	}
	return is_valid;
}

}
