//
// Created by Ludwig Andreas on 23.08.2023.
//

#include "Properties.h"

#include <utility>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "Logger.h"

namespace s21 {

Properties::Properties(std::string separator,
                       std::string comment_symb,
                       std::map<std::string, std::string> properties)
    : separator(std::move(separator)),
      comment_symb(std::move(comment_symb)),
      properties_(std::move(properties)) {}


Properties::Properties() : separator("="), comment_symb("#") {}

void Properties::Load(const std::string& file_name) {
  file_name_ = file_name;

  std::ifstream reader(file_name, std::ios::binary);
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
      std::string value = line.substr(equalPos + separator.size());

      key.erase(0, first_sym);
      key.erase(key.find_last_not_of(' ') + 1);

      value.erase(0, value.find_first_not_of(' '));
      value.erase(value.find_last_not_of(' ') + 1);

      if (value[0] == '\"') {
        value = value.substr(1, value.find_first_of('\"', 1) - 1);
      } else if (comment_pos != std::string::npos) {
        value.erase(comment_pos - key.size() - comment_symb.size());
        value.erase(value.find_last_not_of(' ') + 1);
      }
      key_value_map[key] = value;
    }
  }
  if (reader && reader.is_open())
    reader.close();
  properties_ = key_value_map;
}

std::string Properties::Get(std::string key) {
  return properties_[key];
}

std::string Properties::GetProperty(std::string key) {
  return properties_[key];
}

std::string Properties::GetProperty(std::string key, std::string default_value) {
  try {
    return properties_.at(key);
  } catch (std::out_of_range& e) {
    return default_value;
  }
}

void Properties::SetProperty(const std::string& key, const std::string& value) {
  try {
    properties_.at(key) = value;
  } catch (std::out_of_range& e) {
    properties_.insert({key, value});
  }
}

void Properties::Save() {
  if (file_name_.empty() || !std::filesystem::exists(file_name_)) {
    LOG_TRACE(diagnostic::Logger::getRootLogger(), "Properties loading error: File name is empty or file does not exist");
    return;
  }
  std::string file_name = file_name_ + ".tmp";


  std::map<std::string, std::string> properties = properties_;


  std::ifstream reader(file_name_, std::ios::binary);
  if (reader.bad()) {
    return;
  }
  std::stringstream ss_out;


  std::string line;
  while (std::getline(reader, line)) {
    size_t comment_pos = line.find(comment_symb);
    size_t equal_pos = line.find(separator, 0);
    if (equal_pos != std::string::npos && comment_pos > equal_pos) {
      std::string key = line.substr(0, equal_pos);
      key.erase(0, key.find_first_not_of(' '));
      key.erase(key.find_last_not_of(' ') + 1);
      try {
        std::string value = properties.at(key);
        ss_out << key << " " << separator << " \"" << value << "\"" << std::endl;
        properties.erase(key);
      } catch (std::out_of_range& e) {
        ss_out << line << std::endl;
      }
    } else {
      ss_out << line << std::endl;
    }
  }
  if (reader && reader.is_open())
      reader.close();

  for (const auto& pair : properties) {
    ss_out << pair.first << " " << separator << " " << pair.second << std::endl;
  }

  std::ofstream writer(file_name, std::ios::binary);
  if (writer.bad()) {
    return;
  }
  writer << ss_out.str();
  writer.flush();
  writer.close();
  std::filesystem::rename(file_name, file_name_);
}

// std::string Properties::Get(const std::string& key) {
//   return properties_.at(key);
// }

// std::string Properties::GetProperty(const std::string& key) {
//   return properties_.at(key);
// }

// std::string Properties::GetProperty(const std::string& key, std::string default_value) {
//   try {
//     return properties_.at(key);
//   } catch (std::out_of_range& e) {
//     return default_value;
//   }
// }

std::set<std::string> Properties::StringPropertyNames() {
  std::set<std::string> keys;

  for (const auto& pair : properties_) {
    keys.insert(pair.first);
  }

  return keys;
}

}