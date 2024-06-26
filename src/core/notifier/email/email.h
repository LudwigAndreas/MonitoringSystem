#pragma once

#include <curl/curl.h>

#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include <sstream>
#include <string>

#include "email_address.h"
#include "logger.h"

namespace s21 {

class Email {
 public:
  Email(const EmailAddress &from, const EmailAddresses &to,
        const std::string &subject, const std::string &body)
      : from_{from}, to_{to}, subject_{subject}, body_{body} {}

  CURLcode SendMessage(const std::string &url, const std::string &userName,
                       const std::string &password);

 private:
  struct StringData {
    std::string msg;
    size_t bytes_left;
    StringData(std::string &&m) : msg{m}, bytes_left{msg.size()} {}
    StringData(std::string &m) = delete;
  };

  static std::string DateTimeNow();
  static size_t PayloadSource(void *ptr, size_t size, size_t nmemb,
                              void *userp);
  std::string GenerateMessageId() const;
  std::string SetPayloadText();

  EmailAddress from_;
  EmailAddresses to_;
  std::string subject_, body_;
};

}  // namespace s21