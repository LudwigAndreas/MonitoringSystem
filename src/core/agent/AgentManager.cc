//
// Created by Ludwig Andreas on 31.07.2023.
//

#include "AgentManager.h"

#include <iostream>
#include <chrono>
#include <utility>
#include <set>

#include "AgentBundleLoader.h"

namespace s21 {

namespace fs = std::filesystem;

AgentManager::AgentManager(std::string agents_directory)
    : agents_directory_(std::move(agents_directory)), is_monitoring_(false) {
  app_logger_ = diagnostic::Logger::getRootLogger();
  agent_list_ = new std::map<std::string, AgentBundlePtr>();
}

AgentManager::~AgentManager() {
  StopMonitoring();
  delete agent_list_;
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

void AgentManager::MonitorAgentsDirectory() {
  std::set<fs::path> current_files;

  LOG_TRACE(app_logger_, "Monitoring is running.");
  while (is_monitoring_) {
    try {
      std::set<fs::path> new_files;
      for (const auto &entry: std::filesystem::directory_iterator(
          agents_directory_)) {
        if (entry.is_directory()
            && entry.path().extension() == agent_extension_
            && CheckAgentStructure(entry)) {
          new_files.insert(entry);
        }
      }

      for (const auto &file: new_files) {
        if (current_files.find(file) == current_files.end()) {
          agent_list_->insert({file, AgentBundleLoader::LoadAgentBundle(file)});
          NotifyAgentAdded(agent_list_->at(file));
          LOG_INFO(app_logger_, "Dynamic library added: " << file);
        }
      }

      for (const auto &file: current_files) {
        if (new_files.find(file) == new_files.end()) {
          LOG_INFO(app_logger_, "Dynamic library removed: " << file);
          NotifyAgentRemoved(agent_list_->at(file));
          agent_list_->erase(file);
        }
      }

      current_files = new_files;
    } catch (const std::filesystem::filesystem_error &ex) {
      LOG_ERROR(app_logger_,
                "Unable to open " << agents_directory_
                                  << ". Directory does not exist.");
    }
    std::this_thread::sleep_for(std::chrono::seconds(sleep_duration_));
  }
}

bool AgentManager::CheckAgentStructure(const std::filesystem::path &agent_path) {
  fs::path propertiesFilename = "agent.properties";

  if (agent_path.extension() != agent_extension_ || !fs::exists(agent_path)
      || !fs::is_directory(agent_path)) {
    return false;
  }

  for (const auto &entry: fs::directory_iterator(agent_path)) {
    if (fs::is_regular_file(entry)) {
      fs::path filename = entry.path().filename();
      if (filename.extension() == ".dylib"
          && fs::exists(agent_path / propertiesFilename)) {
        return true;
      }
    }
  }
  return false;
}

std::map<std::string, AgentBundlePtr> *AgentManager::GetAgentList() {
  return agent_list_;
}

void AgentManager::Subscribe(IAgentSubscriber *subscriber) {
  subscribers_.push_back(subscriber);
}

void AgentManager::Unsubscribe(IAgentSubscriber *subscriber) {
  subscribers_.erase(std::remove(subscribers_.begin(), subscribers_.end(),
                                 subscriber), subscribers_.end());

}

void AgentManager::NotifyAgentAdded(AgentBundlePtr agent) {
  for (auto subscriber: subscribers_) {
    subscriber->OnAgentAdded(agent);
  }
}

void AgentManager::NotifyAgentRemoved(AgentBundlePtr agent) {
  for (auto subscriber: subscribers_) {
    subscriber->OnAgentRemoved(agent);
  }
}

void AgentManager::NotifyAgentUpdated(AgentBundlePtr agent) {
  for (auto subscriber: subscribers_) {
    subscriber->OnAgentUpdated(agent);
  }
}

}