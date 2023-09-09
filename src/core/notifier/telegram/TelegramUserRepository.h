#pragma once

#include <string>
#include <stdexcept>
#include <map>

namespace s21 {

class TelegramUserRepository {
private:
	std::map<std::string, long> db;

public:
	TelegramUserRepository(std::map<std::string, long> users = {});
	std::map<std::string, long> GetUsers();
	void AddUser(std::string username, long chat_id);
	int GetUser(std::string username);
	void RemoveUser(std::string username);
};

}