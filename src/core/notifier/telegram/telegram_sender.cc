#include "telegram_sender.h"

s21::TelegramSender::TelegramSender(
    const std::shared_ptr<ITelegramBot> &bot,
    const std::shared_ptr<IUserRepository> &users, std::string receivers)
    : bot(bot), users(users), receivers() {
  polling_thread = std::make_shared<std::thread>();
  polling_thread.reset();

  InitializeReceivers(receivers);
  ConfigPolling();
  // StartPolling();
  for (auto &receiver : this->receivers) {
    if (std::atoi(users->GetUser(receiver).c_str()) == 0) {
      StartPolling();
      break;
    }
  }
}

s21::TelegramSender::~TelegramSender() { StopPolling(); }

void s21::TelegramSender::InitializeReceivers(std::string receivers) {
  std::stringstream ss(receivers);
  std::string receiver;

  while (ss >> receiver) {
    std::transform(receiver.begin(), receiver.end(), receiver.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    this->receivers.insert(receiver);
  }
}

void s21::TelegramSender::ConfigPolling() {
  bot->OnStart([this](TgBot::Message::Ptr message) {
    std::string chat_id = std::to_string(message->chat->id);
    std::string username = message->from->username;
    std::transform(username.begin(), username.end(), username.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    this->bot->SendMessage(chat_id, "You are now registered in MSBOT!");
    this->users->AddUser(username, chat_id);
    LOG_DEBUG(diagnostic::Logger::getRootLogger(),
              "Received /start command from " << username << ":" << chat_id)
  });
}

void s21::TelegramSender::StartPolling() {
  is_polling_running = true;
  // if (!polling_thread)
  polling_thread =
      std::make_shared<std::thread>(&TelegramSender::PollingFunction, this);
}

void s21::TelegramSender::StopPolling() {
  if (is_polling_running) {
    is_polling_running = false;
  }
  if (polling_thread && polling_thread->joinable()) {
    polling_thread->join();
    polling_thread.reset();
  }
}

std::string s21::TelegramSender::PrepareMessage(FailedMetric fm) {
  std::ostringstream ss;
  ss << "Metric: "
     << "\"" << fm.metric_name << "\""
     << "\n"
     << "Hostname: "
     << "[" << this->hostname << "]"
     << "\n"
     << "Date: " << fm.date /*<< "\n"*/
     << "Value: " << fm.value << "\n"
     << "Critical value: "
     << "\"" << fm.critical_value << "\""
     << ".";
  return ss.str();
}

void s21::TelegramSender::SendMessage(FailedMetric fm) {
  if (enabled_) {
    std::string message = PrepareMessage(fm);
    for (auto &receiver : receivers) {
      std::string chat_id = users->GetUser(receiver);
      if (std::atoi(users->GetUser(receiver).c_str())) {
        bot->SendMessage(chat_id, message);
      }
    }
  }
}

void s21::TelegramSender::SetEnabled(bool enabled) { enabled_ = enabled; }

std::set<std::string> s21::TelegramSender::GetRecievers() { return receivers; }

void s21::TelegramSender::AddReceiver(std::string username,
                                      std::string chat_id) {
  std::transform(username.begin(), username.end(), username.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  receivers.insert(username);
  if (std::atoi(chat_id.c_str())) users->AddUser(username, chat_id);
  PollingFunctionCheck();
}

void s21::TelegramSender::RemoveReceiver(std::string username) {
  receivers.erase(username);
  PollingFunctionCheck();
}

void s21::TelegramSender::RemoveReceivers() {
  receivers = std::set<std::string>();
  StopPolling();
}

// TODO: Possible mangled data
bool s21::TelegramSender::PollingCheck() {
  for (auto &receiver : receivers) {
    LOG_DEBUG(s21::diagnostic::Logger::getRootLogger(),
              receiver << ": " << users->GetUser(receiver));
    if (std::atoi(users->GetUser(receiver).c_str()) == 0) return true;
  }
  return false;
}

void s21::TelegramSender::PollingFunctionCheck() {
  if (PollingCheck())
    StartPolling();
  else
    StopPolling();
}

void s21::TelegramSender::PollingFunction() {
  while (is_polling_running) {
    LOG_DEBUG(s21::diagnostic::Logger::getRootLogger(), "Still polling");
    bot->LongPoll();
    is_polling_running = PollingCheck();
  }
}