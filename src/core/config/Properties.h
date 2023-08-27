//
// Created by Ludwig Andreas on 23.08.2023.
//

#ifndef MONITORINGSYSTEM_SRC_CORE_CONFIG_PROPERTIES_H_
#define MONITORINGSYSTEM_SRC_CORE_CONFIG_PROPERTIES_H_

#include <set>
#include <map>
#include <istream>

namespace s21 {

class Properties {
 public:
  Properties(std::string separator,
             std::string comment_symb,
             std::map<std::string, std::string> properties);

  Properties();

  void Load(std::istream& reader);

  std::string Get(std::string key);
  // std::string Get(const std::string& key);

  std::string GetProperty(std::string key);
  // std::string GetProperty(const std::string& key);

  std::string GetProperty(std::string key, std::string default_value);
  // std::string GetProperty(const std::string& key, std::string default_value);

  std::set<std::string> StringPropertyNames();

 private:

  std::string separator = "=";
  std::string comment_symb = "#";
  std::map<std::string, std::string> properties_;
};

}

#endif //MONITORINGSYSTEM_SRC_CORE_CONFIG_PROPERTIES_H_
