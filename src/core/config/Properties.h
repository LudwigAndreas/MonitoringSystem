//
// Created by Ludwig Andreas on 23.08.2023.
//

#ifndef MONITORINGSYSTEM_SRC_CORE_CONFIG_PROPERTIES_H_
#define MONITORINGSYSTEM_SRC_CORE_CONFIG_PROPERTIES_H_

#include <istream>
#include <map>
#include <set>

namespace s21 {

class Properties {
 public:
  Properties(std::string separator, std::string comment_symb,
             std::map<std::string, std::string> properties);

  Properties();

  void Load(const std::string& file_name);

  std::string Get(std::string key);

  std::string GetProperty(std::string key);

  std::string GetProperty(std::string key, std::string default_value);

  void SetProperty(const std::string& key, const std::string& value);

  void Save();

  std::set<std::string> StringPropertyNames();

 private:
  std::string separator = "=";
  std::string comment_symb = "#";
  std::map<std::string, std::string> properties_;
  std::string file_name_;
};

}  // namespace s21

#endifMONITORINGSYSTEM_SRC_CORE_CONFIG_PROPERTIES_H_
