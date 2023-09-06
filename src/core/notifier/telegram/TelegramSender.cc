#include "TelegramSender.h"

TelegramSender::TelegramSender() : bot(__token) {
  bot.getEvents().onAnyMessage([this](TgBot::Message::Ptr message) {
    this->bot.getApi().sendMessage(
      message->chat->id,
      "idk lol"
    );
    if (message->text == "/start") {
      this->users.addUser(
        message->from->username,
        message->chat->id
      );
      this->bot.getApi().sendMessage(
        message->chat->id,
        "You are now registered in MSBOT!"
      );
    }
  });
  is_polling_running = true;
  polling_thread = std::thread(&TelegramSender::PollingFunction, this);
}

TelegramSender::~TelegramSender() {
  if (is_polling_running) {
    is_polling_running = false;
  }
  if (polling_thread.joinable()) {
    polling_thread.join();
  }
}

std::string TelegramSender::prepareMessage(FailedMetric fm) {
  std::ostringstream ss;
  ss  << "Metric: "         << "\"" << fm.metric_name << "\""     << "\n"
    << "Hostname: "       << "[" << this->hostname << "]"       << "\n"
    << "Date: "           << fm.date                            /*<< "\n"*/
    << "Value: "          << fm.value                           << "\n"
    << "Critical value: " << "\"" << fm.critical_value << "\""  << ".";
  return ss.str();
}

void TelegramSender::sendMessage(FailedMetric fm) {
  std::string message = prepareMessage(fm);
  for (auto receiver: receivers) {
    int chat_id = users.getUser(receiver);
    if (chat_id != DEFAULT_USER) {
      bot.getApi().sendMessage(
          chat_id,
          message
      );
    }
  }
}

std::set<std::string> TelegramSender::getReceivers() {
  return receivers;
}

void TelegramSender::addReceiver(std::string username, long chat_id) {
  receivers.insert(username);
  if (chat_id != TelegramSender::DEFAULT_USER)
    users.addUser(username, chat_id);
}

void TelegramSender::removeReceiver(std::string username) {
  receivers.erase(username);
}

void TelegramSender::PollingFunction() {
  TgBot::TgLongPoll longPoll(bot);
  while (is_polling_running) {
    longPoll.start();
  }
}