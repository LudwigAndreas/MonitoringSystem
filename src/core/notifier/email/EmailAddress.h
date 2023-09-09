#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <random>
#include <curl/curl.h>

class EmailAddress 
{
public:
    EmailAddress(const char *email);
    EmailAddress(const std::string &email, const std::string &display_name = "");
    std::string Domain() const;
    explicit operator const char *() const;
    friend std::ostream &operator<<(std::ostream &out, const EmailAddress &email);

private:
    std::string email_;
    std::string name_;
};

using EmailAddresses = std::vector<EmailAddress>;

std::ostream &operator<<(std::ostream &out, const EmailAddresses &emailAddresses);