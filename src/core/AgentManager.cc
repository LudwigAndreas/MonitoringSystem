//
// Created by Ludwig Andreas on 31.07.2023.
//

#include "AgentManager.h"

#include <iostream>
#include <chrono>
#include <filesystem>
#include <utility>

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

s21::Agent *AgentManager::Instantiate(const DynamicLib lib) {
  void *maker = dlsym(lib, lib_entry_point_.c_str());

  if (maker == nullptr)
    return nullptr;

  typedef s21::Agent *(*fptr)();
  fptr func = reinterpret_cast<fptr>(reinterpret_cast<void *>(maker));

  return func();
}

void AgentManager::MonitorAgentsDirectory() {
  while (is_monitoring_) {
    LOG_TRACE(app_logger_, "Monitoring is running.");
    try {
      for (const auto &entry: std::filesystem::directory_iterator(
          agents_directory_)) {
        if (entry.is_regular_file() && entry.path().extension()
            == lib_extension_) {
          std::string library_path = entry.path().string();
          LOG_INFO(app_logger_, "Found dynamic library: " << library_path);
          std::string absolute_path = std::filesystem::absolute(library_path).string();

          void *lib_handle = dlopen(absolute_path.c_str(), RTLD_LAZY);
          if (lib_handle) {
            s21::Agent *agent = Instantiate(lib_handle);
            if (agent) {
              agent_list_.push_back(agent);
              LOG_INFO(app_logger_,
                       "Agent successfully loaded: " << agent->GetAgentName()
                                                     << " "
                                                     << agent->GetAgentType());
            } else {
              LOG_ERROR(app_logger_, "Agent loading error: " << dlerror());
            }
            dlclose(lib_handle);
          } else {
            LOG_ERROR(app_logger_,
                      "Failed to load the dynamic library: " << dlerror());
          }
        }
      }
    } catch (const std::filesystem::filesystem_error &ex) {
      LOG_ERROR(app_logger_, "Unable to open " << agents_directory_ << ". Directory does not exist.");
    }
    std::this_thread::sleep_for(std::chrono::seconds(sleep_duration_)); // Adjust the sleep time as needed
  }
}

}