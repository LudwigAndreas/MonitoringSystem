//
// Created by Ludwig Andreas on 31.07.2023.
//

#include "agent_manager.h"

#include <chrono>
#include <filesystem>
#include <iostream>
#include <set>
#include <utility>

#include "agent_bundle_loader.h"

#ifdef _WIN32
#define LIB_EXTENSION ".dll"
#elif defined(__linux__)
#define LIB_EXTENSION ".so"
#elif defined(__APPLE__)
#define LIB_EXTENSION ".dylib"
#else
#error Unsupported platform
#endif

namespace s21 {

namespace fs = std::filesystem;

AgentManager::AgentManager(std::string agents_directory, size_t sleep_duration)
    : sleep_duration_(sleep_duration),
      agents_directory_(std::move(agents_directory)),
      is_monitoring_(false) {
  app_logger_ = diagnostic::Logger::getRootLogger();
  agent_list_ = new std::map<std::string, AgentBundlePtr>();
}

AgentManager::~AgentManager() {
  StopMonitoring();
  if (monitoring_thread_.joinable()) {
    monitoring_thread_.join();
  }
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
  }
}

bool AgentManager::IsMonitoring() { return is_monitoring_; }

void AgentManager::MonitorAgentsDirectory() {
  std::set<fs::path> current_files;

  LOG_TRACE(app_logger_, "Monitoring is running.");
  while (is_monitoring_) {
    try {
      std::set<fs::path> new_files;
      for (const auto &entry :
           std::filesystem::directory_iterator(agents_directory_)) {
        if (entry.is_directory() &&
            entry.path().extension() == agent_extension_ &&
            CheckAgentStructure(entry)) {
          new_files.insert(entry);
        }
      }

      for (const auto &file : current_files) {
        if (new_files.find(file) == new_files.end()) {
          LOG_INFO(app_logger_, "Dynamic library removed: " << file);
          NotifyAgentRemoved(agent_list_->at(file));
          agent_list_->erase(file);
        }
      }

      for (const auto &file : new_files) {
        if (current_files.find(file) == current_files.end()) {
          agent_list_->insert({file, AgentBundleLoader::LoadAgentBundle(file)});
          NotifyAgentAdded(agent_list_->at(file));
          LOG_INFO(app_logger_, "Dynamic library added: " << file);
        }
      }


      for (const auto &agent: *agent_list_) {
        if (agent.second.get()) {
          time_t last_modif = agent.second->GetLastModified();
          std::shared_ptr<AgentBundle> agent_bundle =
              AgentBundleLoader::UpdateAgentBundle(agent.first, agent.second);
          
          if (agent_bundle == nullptr) {
            LOG_INFO(app_logger_, "Dynamic library removed: " << agent_bundle->GetAgentPath());
            NotifyAgentRemoved(agent.second);
            agent_list_->erase(agent.first);
          } else if (agent_bundle.get() && agent_bundle->GetLastModified() != last_modif) {
            LOG_INFO(app_logger_, "Dynamic library updated: " << agent_bundle->GetAgentPath());
            NotifyAgentUpdated(agent_bundle);
          }
        }
      }

      current_files = new_files;
    } catch (const std::filesystem::filesystem_error &ex) {
      fs::create_directory(agents_directory_);
    }
    Sleep(std::chrono::seconds(sleep_duration_));
  }
}

bool AgentManager::CheckAgentStructure(
    const std::filesystem::path &agent_path) {
  fs::path propertiesFilename = "agent.properties";

  if (agent_path.extension() != agent_extension_ || !fs::exists(agent_path) ||
      !fs::is_directory(agent_path)) {
    return false;
  }

  for (const auto &entry : fs::directory_iterator(agent_path)) {
    if (fs::is_regular_file(entry)) {
      fs::path filename = entry.path().filename();
      if (filename.extension() == LIB_EXTENSION &&
          fs::exists(agent_path / propertiesFilename)) {
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
  subscribers_.erase(
      std::remove(subscribers_.begin(), subscribers_.end(), subscriber),
      subscribers_.end());
}

void AgentManager::NotifyAgentAdded(AgentBundlePtr agent) {
  for (auto subscriber : subscribers_) {
    subscriber->OnAgentAdded(agent);
  }
}

void AgentManager::NotifyAgentRemoved(AgentBundlePtr agent) {
  for (auto subscriber : subscribers_) {
    subscriber->OnAgentRemoved(agent);
  }
}

void AgentManager::NotifyAgentUpdated(AgentBundlePtr agent) {
  for (auto subscriber : subscribers_) {
    subscriber->OnAgentUpdated(agent);
  }
}

void AgentManager::DeleteAgent(std::shared_ptr<AgentBundle> &agent) {
  std::string agent_path = agent->GetAgentPath();
  if (agent_list_->find(agent_path) != agent_list_->end()) {
    std::filesystem::remove_all(agent_path);
  }
}

void AgentManager::Sleep(std::chrono::milliseconds sleep_time) {
  auto start_time = std::chrono::steady_clock::now();
  auto half_second = std::chrono::milliseconds(500);

  while (is_monitoring_) {
    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - start_time);

    if (elapsed_time >= sleep_time) {
      break;  // Время сна истекло, выходим из цикла
    }

    std::this_thread::sleep_for(half_second);
  }
}

}  // namespace s21