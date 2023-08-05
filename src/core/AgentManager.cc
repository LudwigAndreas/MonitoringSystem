//
// Created by Ludwig Andreas on 31.07.2023.
//

#include "AgentManager.h"

#include <iostream>
#include <chrono>
#include <filesystem>
#include <utility>
#include <set>

#if __linux__ || __APPLE__
#include <dlfcn.h>
#elif _WIN32
#include <windows.h>
#endif

namespace s21 {

AgentManager::AgentManager(std::string agents_directory)
    : agents_directory_(std::move(agents_directory)), is_monitoring_(false) {
  app_logger_ = diagnostic::Logger::getRootLogger();
}

void AgentManager::StartMonitoring() {
  if (!is_monitoring_) {
    is_monitoring_ = true;
    monitoring_thread_ =
        std::thread(&AgentManager::MonitorAgentsDirectory, this);
  }
}

void AgentManager::StopMonitoring() {
  if (is_monitoring_) {
    is_monitoring_ = false;
    if (monitoring_thread_.joinable()) {
      monitoring_thread_.join();
    }
  }
}

std::shared_ptr<Agent> AgentManager::Instantiate(DynamicLib lib) {
  if (!lib)
    return nullptr;
  void *maker = dlsym(lib, lib_entry_point_.c_str());

  if (maker == nullptr)
    return nullptr;

  typedef s21::Agent *(*fptr)();
  fptr func = reinterpret_cast<fptr>(reinterpret_cast<void *>(maker));

  return std::shared_ptr<Agent>(func(), [](Agent *agent) {
    delete agent;
  });
}

void AgentManager::MonitorAgentsDirectory() {
  std::set<std::string> current_files;

  LOG_TRACE(app_logger_, "Monitoring is running.");
  while (is_monitoring_) {
    try {
      std::set<std::string> new_files;
      for (const auto &entry: std::filesystem::directory_iterator(agents_directory_)) {
        if (entry.is_regular_file() && entry.path().extension() == lib_extension_) {
          std::string library_path = entry.path().string();
          new_files.insert(library_path);
        }
      }

      // Check for new files
      for (const auto &file : new_files) {
        if (current_files.find(file) == current_files.end()) {
          LOG_INFO(app_logger_, "New dynamic library added: " << file);
          std::string absolute_path = std::filesystem::absolute(file).string();
          DynamicLib lib_handle = dlopen(absolute_path.c_str(), RTLD_LAZY);
          if (lib_handle) {
            AgentPtr agent = Instantiate(lib_handle);
            if (agent) {
              agent_list_.insert({file, agent});
              LOG_INFO(app_logger_, "Agent was successfully loaded: " <<
                                                                      "Agent name: \"" <<
                                                                      agent->GetAgentName() << "\" |Agent type \"" <<
                                                                      agent->GetAgentType() << "\"");
            } else {
              LOG_ERROR(app_logger_, "Agent loading error: " << dlerror());
            }
          } else {
            LOG_ERROR(app_logger_, "Failed to load the dynamic library: " << dlerror());
          }
        }
      }

      // Check for removed files
      for (const auto &file : current_files) {
        if (new_files.find(file) == new_files.end()) {
          LOG_INFO(app_logger_, "Dynamic library removed: " << file);
          // You might want to remove the agent from the agent_list_ as well
          agent_list_.erase(file);
        }
      }

      current_files = std::move(new_files); // Update the current set with the new set
    } catch (const std::filesystem::filesystem_error &ex) {
      LOG_ERROR(app_logger_, "Unable to open " << agents_directory_ << ". Directory does not exist.");
    }
    std::this_thread::sleep_for(std::chrono::seconds(sleep_duration_));
  }
}



}