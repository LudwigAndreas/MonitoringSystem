#include "TelegramUserRepository.h"

std::map<std::string, long> TelegramUserRepository::getUsers() {
	return db;
}

void TelegramUserRepository::addUser(std::string username, long chat_id) {
	db[username] = chat_id;
}

int TelegramUserRepository::getUser(std::string username) {
	try {
		return db.at(username);
	} catch (std::out_of_range& e) {
		return -1;
	}
}

void TelegramUserRepository::removeUser(std::string username) {
	db.erase(username);
}

// };