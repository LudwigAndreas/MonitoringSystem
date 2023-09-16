#pragma once

#include <stdexcept>

#include "IUserRepository.h"
#include "config/Properties.h"

namespace s21 {

#define EMPTY_CHAT_ID ""

class TelegramUserRepository : public IUserRepository {
private:
	Properties db;

public:
	TelegramUserRepository(const Properties& prop = {});
	// std::map<std::string, std::string> GetUsers();
	virtual void AddUser(std::string username, std::string chat_id) override;
	virtual std::string GetUser(std::string username) override;
	// void RemoveUser(std::string username);
};

}