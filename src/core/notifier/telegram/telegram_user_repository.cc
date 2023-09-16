#include "telegram_user_repository.h"

namespace s21 {

TelegramUserRepository::TelegramUserRepository(const Properties& prop)
    : db(prop) {}

void TelegramUserRepository::AddUser(std::string username,
                                     std::string chat_id) {
  db.SetProperty(username, chat_id);
  db.Save();
}

std::string TelegramUserRepository::GetUser(std::string username) {
  return db.GetProperty(username, EMPTY_CHAT_ID);
}

}  // namespace s21