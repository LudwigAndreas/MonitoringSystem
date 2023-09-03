//
// Created by Ludwig Andreas on 23.08.2023.
//

#include "Properties.h"

#include <utility>

namespace s21 {

Properties::Properties(std::string separator,
                       std::string comment_symb,
                       std::map<std::string, std::string> properties)
    : separator(std::move(separator)),
      comment_symb(std::move(comment_symb)),
      properties_(std::move(properties)) {}


Properties::Properties() : separator("="), comment_symb("#") {}

void Properties::Load(std::istream &reader) {
  if (reader.bad()) {
    properties_ = {};
  }

  std::map<std::string, std::string> key_value_map;
  std::string line;
  while (std::getline(reader, line)) {
    size_t comment_pos = line.find(comment_symb);
    size_t equalPos = line.find(separator, 0);

    size_t first_sym = line.find_first_not_of(' ');
    if (equalPos != std::string::npos && equalPos != first_sym
        && comment_pos > equalPos) {

      std::string key = line.substr(0, equalPos);
      std::string value = line.substr(equalPos + 1);

      key.erase(0, first_sym);
      key.erase(key.find_last_not_of(' ') + 1);

      value.erase(0, value.find_first_not_of(' '));
      value.erase(value.find_last_not_of(' ') + 1);

      if (value[0] == '\"') {
        value = value.substr(1, value.find_first_of('\"', 1) - 1);
      } else if (comment_pos != std::string::npos) {
        value.erase(comment_pos - key.size() - 1);
        value.erase(value.find_last_not_of(' ') + 1);
      }
      key_value_map[key] = value;
    }
  }
  properties_ = key_value_map;
}

std::string Properties::Get(std::string key) {
  return properties_.at(key);
}

std::string Properties::GetProperty(std::string key) {
  return properties_.at(key);
}

std::string Properties::GetProperty(std::string key, std::string default_value) {
  try {
    return properties_.at(key);
  } catch (std::out_of_range& e) {
    return default_value;
  }
}

std::set<std::string> Properties::StringPropertyNames() {
  std::set<std::string> keys;

  for (const auto& pair : properties_) {
    keys.insert(pair.first);
  }

  return keys;
}

}