#pragma once

#include <string>
#include <stdexcept>
#include <map>

class TelegramUserRepository {
private:
	std::map<std::string, long> db;

public:
	std::map<std::string, long> getUsers();
	void addUser(std::string username, long chat_id);
	int getUser(std::string username);
	void removeUser(std::string username);
};