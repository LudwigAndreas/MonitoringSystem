#include "TelegramSender.h"

s21::TelegramSender::TelegramSender(std::string receivers) : 
  properties(config_file), bot(properties.Get("config.token")) {
  InitializeReceivers(receivers);
  InitializeRepository();
  ConfigPolling();

  // if (users.GetUser(std::string username))
  StartPolling();
}

s21::TelegramSender::~TelegramSender() {
  StopPolling();
}

void s21::TelegramSender::InitializeRepository() {
  
}

void s21::TelegramSender::InitializeReceivers(std::string receivers) {
  std::stringstream ss(receivers);
  std::string receiver;

  while (ss >> receiver) {
    std::transform(
      receiver.begin(),
      receiver.end(),
      receiver.begin(),
      [](unsigned char c) {
        return std::tolower(c);
      }
    );
    this->receivers.insert(receiver);
  }
}

void s21::TelegramSender::ConfigPolling() {
  bot.getEvents().onCommand("start",
    [this](TgBot::Message::Ptr message) {
      this->users.AddUser(
        message->from->username,
        message->chat->id
      );
      this->bot.getApi().sendMessage(
        message->chat->id,
        "You are now registered in MSBOT!"
      );
    }
  );
}

void s21::TelegramSender::StartPolling() {
  is_polling_running = true;
  polling_thread = std::thread(&TelegramSender::PollingFunction, this);
}

void s21::TelegramSender::StopPolling() {
  if (is_polling_running) {
    is_polling_running = false;
  }
  if (polling_thread.joinable()) {
    polling_thread.join();
  }
}

std::string s21::TelegramSender::PrepareMessage(FailedMetric fm) {
  std::ostringstream ss;
  ss  << "Metric: "         << "\"" << fm.metric_name << "\""     << "\n"
    << "Hostname: "       << "[" << this->hostname << "]"       << "\n"
    << "Date: "           << fm.date                            /*<< "\n"*/
    << "Value: "          << fm.value                           << "\n"
    << "Critical value: " << "\"" << fm.critical_value << "\""  << ".";
  return ss.str();
}

void s21::TelegramSender::SendMessage(FailedMetric fm) {
  std::string message = PrepareMessage(fm);
  for (auto receiver: receivers) {
    int chat_id = users.GetUser(receiver);
    if (chat_id != DEFAULT_USER) {
      bot.getApi().sendMessage(
        chat_id,
        message
      );
    }
  }
}

std::set<std::string> s21::TelegramSender::GetRecievers() {
  return receivers;
}

void s21::TelegramSender::AddReceiver(std::string username, long chat_id) {
  receivers.insert(username);
  if (chat_id != s21::TelegramSender::DEFAULT_USER)
    users.AddUser(username, chat_id);
}

void s21::TelegramSender::RemoveReceiver(std::string username) {
  receivers.erase(username);
}

void s21::TelegramSender::PollingFunction() {
  TgBot::TgLongPoll longPoll(bot);
  while (is_polling_running) {
    longPoll.start();
  }
}