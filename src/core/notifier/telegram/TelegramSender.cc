#include "TelegramSender.h"

TelegramSender::TelegramSender() : bot(__token) {
  bot.getEvents().onCommand("start", [this](TgBot::Message::Ptr message) {
    this->users.addUser(
        message->from->username,
        message->chat->id
    );
    this->bot.getApi().sendMessage(message->chat->id,
                                   "You are now registered in MSBOT!");
  });
}

std::string TelegramSender::prepareMessage(FailedMetric fm) {
  std::ostringstream ss;
  ss << "Metric \""
     << fm.metric_name
     << "\" failed at "
     << fm.date
     << " with value of "
     << fm.value
     << " and critical value threshold \""
     << fm.critical_value
     << "\".";
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

void TelegramSender::addReceiver(std::string username, int chat_id) {
  receivers.insert(username);
  if (chat_id != TelegramSender::DEFAULT_USER)
    users.addUser(username, chat_id);
}

void TelegramSender::removeReceiver(std::string username) {
  receivers.erase(username);
}