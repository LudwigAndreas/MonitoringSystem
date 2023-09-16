#pragma once

#include <curl/curl.h>

#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include <sstream>
#include <string>

namespace s21 {

class EmailAddress {
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

std::ostream &operator<<(std::ostream &out,
                         const EmailAddresses &emailAddresses);

}  // namespace s21