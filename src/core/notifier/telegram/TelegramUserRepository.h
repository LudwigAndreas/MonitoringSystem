#pragma once

#include <string>
#include <stdexcept>
#include <map>

class TelegramUserRepository {
private:
	std::map<std::string, int> db;

public:
	std::map<std::string, int> getUsers();
	void addUser(std::string username, int chat_id);
	int getUser(std::string username);
	void removeUser(std::string username);
};