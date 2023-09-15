#include "TelegramSender.h"

const std::string s21::TelegramSender::DEFAULT_USER = "";

s21::TelegramSender::TelegramSender(const std::shared_ptr<ITelegramBot> &bot, const std::shared_ptr<IUserRepository> &users, std::string receivers) : 
  properties(), bot(std::move(bot)), users(std::move(users)) {
  properties.Load("config/telegram.properties");

  polling_thread = std::make_shared<std::thread>();
  polling_thread.reset();
  
  InitializeReceivers(receivers);
  InitializeRepository();
  
  ConfigPolling();

  for (auto &receiver : this->receivers) {
    if (this->users->GetUser(receiver) == DEFAULT_USER) {
      StartPolling();
      break;
    }
  }
}

s21::TelegramSender::~TelegramSender() {
  StopPolling();
}

void s21::TelegramSender::InitializeRepository() {
  for (auto & receiver : receivers) {
    UpdateRepository(receiver);
  }
}

void s21::TelegramSender::UpdateRepository(const std::string &receiver) {
  users->AddUser(receiver,
    properties.GetProperty(
      receiver,
      DEFAULT_USER
    )
  );
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
  bot->OnStart(
    [this](TgBot::Message::Ptr message) {
      std::string chat_id = std::to_string(message->chat->id);
      std::string username;
      std::transform(
        message->from->username.begin(),
        message->from->username.end(),
        username.begin(),
        [](unsigned char c) {
          return std::tolower(c);
        }
      );
      this->users->AddUser(
        username,
        chat_id
      );
      this->bot->SendMessage(
        chat_id,
        "You are now registered in MSBOT!"
      );
      this->properties.SetProperty(
        username, 
        std::to_string(
          message->chat->id
        )
      );
      this->properties.Save();
    }
  );
}

void s21::TelegramSender::StartPolling() {
  is_polling_running = true;
  if (!polling_thread)
    polling_thread = std::make_shared<std::thread>(&TelegramSender::PollingFunction, this);
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
  ss  << "Metric: "         << "\"" << fm.metric_name << "\""     << "\n"
    << "Hostname: "       << "[" << this->hostname << "]"       << "\n"
    << "Date: "           << fm.date                            /*<< "\n"*/
    << "Value: "          << fm.value                           << "\n"
    << "Critical value: " << "\"" << fm.critical_value << "\""  << ".";
  return ss.str();
}

void s21::TelegramSender::SendMessage(FailedMetric fm) {
  if (enabled_) {
    std::string message = PrepareMessage(fm);
    for (auto receiver: receivers) {
      std::string chat_id = users->GetUser(receiver);
      if (chat_id != DEFAULT_USER) {
        bot->SendMessage(
          chat_id,
          message
        );
      }
    }
  }
}

void s21::TelegramSender::SetEnabled(bool enabled) {
  enabled_ = enabled;
}

std::set<std::string> s21::TelegramSender::GetRecievers() {
  return receivers;
}

void s21::TelegramSender::AddReceiver(std::string username, std::string chat_id) {
  std::transform(
    username.begin(),
    username.end(),
    username.begin(),
    [](unsigned char c) {
      return std::tolower(c);
    }
  );
  receivers.insert(username);
  if (chat_id != s21::TelegramSender::DEFAULT_USER)
    users->AddUser(username, chat_id);
  UpdateRepository(username);
  PollingFunctionCheck();
}

void s21::TelegramSender::RemoveReceiver(std::string username) {
  receivers.erase(username);
  PollingFunctionCheck();
}

// TODO: Possible mangled data
bool s21::TelegramSender::PollingCheck() {
  for (auto &receiver : receivers) {
    if (users->GetUser(receiver) == DEFAULT_USER)
      return true;
  }
  return false;
}

void s21::TelegramSender::PollingFunctionCheck() {
  if (PollingCheck())
    StartPolling();
  StopPolling();
}

void s21::TelegramSender::PollingFunction() {
  while (is_polling_running) {
    bot->LongPoll();
    is_polling_running = PollingCheck();
  }
}