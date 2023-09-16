#include "TelegramBot.h"

namespace s21 {

TelegramBot::TelegramBot(std::string token) : bot(token), poll(bot) {}

void TelegramBot::OnStart(const std::function<void(const TgBot::Message::Ptr)> &action) {
	bot.getEvents().onCommand("start", action);
}

void TelegramBot::SendMessage(const std::string& chat_id, const std::string& message) {
	bot.getApi().sendMessage(chat_id, message);
}

void TelegramBot::LongPoll() {
	poll.start();
}

}
