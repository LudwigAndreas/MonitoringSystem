#include "TelegramUserRepository.h"

namespace s21 {

TelegramUserRepository::TelegramUserRepository(std::map<std::string, long> users) : db(std::move(users)) {}

std::map<std::string, long> TelegramUserRepository::GetUsers() {
	return db;
}

void TelegramUserRepository::AddUser(std::string username, long chat_id) {
	db[username] = chat_id;
}

int TelegramUserRepository::GetUser(std::string username) {
	try {
		return db.at(username);
	} catch (std::out_of_range& e) {
		return -1;
	}
}

void TelegramUserRepository::RemoveUser(std::string username) {
	db.erase(username);
}

}