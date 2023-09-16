#pragma once

#include <string>

namespace s21 {

class IUserRepository {
public:
	virtual void AddUser(std::string username, std::string chat_id) = 0;
	virtual std::string GetUser(std::string username) = 0;
};

}