//
// Created by Ludwig Andreas on 24.08.2023.
//

#include "agent_bundle_loader.h"

#include <sys/stat.h>

#include <ctime>

#if __linux__ || __APPLE__
#include <dlfcn.h>

#include <memory>
#elif _WIN32
#include <windows.h>
#endif
#include <fstream>

#include "config/properties.h"
#include "logger.h"

#if __APPLE__
#define DYLIB_EXTENSION ".dylib"
#elif _WIN32
#define DYLIB_EXTENSION ".dll"
#else
#define DYLIB_EXTENSION ".so"
#endif

namespace s21 {

const std::string AgentBundleLoader::lib_extension_ = DYLIB_EXTENSION;
const std::string AgentBundleLoader::lib_entry_point_ = "get_agent";
const std::string AgentBundleLoader::agent_properties_path_ =
    "agent.properties";

std::shared_ptr<Agent> AgentBundleLoader::Instantiate(DynamicLib lib) {
  if (!lib) return nullptr;
  void *maker = dlsym(lib, lib_entry_point_.c_str());

  if (maker == nullptr) return nullptr;

  typedef s21::Agent *(*fptr)();
  fptr func = reinterpret_cast<fptr>(reinterpret_cast<void *>(maker));
  auto *agent = reinterpret_cast<Agent *>(func());
  if (!agent) return nullptr;
  return std::shared_ptr<Agent>(agent, [](Agent *agent) { delete agent; });
}

std::shared_ptr<AgentBundle> AgentBundleLoader::LoadAgentBundle(
    const std::filesystem::path &agent_path) {
  diagnostic::LoggerPtr app_logger_ = diagnostic::Logger::getRootLogger();

  std::string library_path = FindLibraryInBundle(agent_path);
  std::string properties_path = agent_path / agent_properties_path_;

  if (library_path.empty() || !fs::exists(properties_path)) {
    LOG_WARN(app_logger_, "Wrong content of AgentBundle: "
                              << agent_path.filename()
                              << ": Check existence of the Agent library"
                              << " and the agent.properties file");
    return nullptr;
  }

  LOG_INFO(app_logger_, "New Agent found: " << agent_path.filename());
  std::string absolute_path = std::filesystem::absolute(library_path).string();
  DynamicLib lib_handle = dlopen(absolute_path.c_str(), RTLD_LAZY);
  if (lib_handle) {
    AgentPtr agent = Instantiate(lib_handle);
    PropertiesPtr properties =
        LoadProperties(agent_path / agent_properties_path_);
    std::shared_ptr<AgentBundle> agent_bundle =
        std::make_shared<AgentBundle>(agent, properties, agent_path);
    if (agent && properties) {
      LOG_INFO(app_logger_, "Agent was successfully loaded: "
                                << "Agent name: \"" << agent->GetAgentName()
                                << "\" |Agent type \"" << agent->GetAgentType()
                                << "\"");
      struct stat fileStat;
      time_t last_modified;

      if (stat(absolute_path.c_str(), &fileStat) == 0) {
        last_modified = fileStat.st_mtime;
      }
      if (stat(properties_path.c_str(), &fileStat) == 0 &&
          fileStat.st_mtime > last_modified) {
        last_modified = fileStat.st_mtime;
      }
      agent_bundle->SetLastModified(last_modified);
      return agent_bundle;
    } else {
      LOG_ERROR(app_logger_,
                "Agent loading error."
                " Check the contents of the Agent"
                " and the existence of the agent.properties file");
    }
  } else {
    LOG_ERROR(app_logger_, "Failed to load the dynamic library: " << dlerror());
  }
  return nullptr;
}

std::shared_ptr<AgentBundle> AgentBundleLoader::UpdateAgentBundle(
    const std::filesystem::path &agent_path,
    const std::shared_ptr<AgentBundle> &agent_bundle) {
  diagnostic::LoggerPtr app_logger_ = diagnostic::Logger::getRootLogger();
  std::string library_path = FindLibraryInBundle(agent_path);
  std::string properties_path = agent_path / agent_properties_path_;

  if (library_path.empty() || !fs::exists(properties_path)) {
    LOG_WARN(app_logger_, "Wrong content of AgentBundle: "
                              << agent_path.filename()
                              << ": Check existence of the Agent library"
                              << " and the agent.properties file");
    return nullptr;
  }

  struct stat fileStat;

  if (stat(library_path.c_str(), &fileStat) == 0 &&
      fileStat.st_mtime > agent_bundle->GetLastModified()) {
    LOG_DEBUG(app_logger_,
              "Agent library (lib file) was updated: " << agent_path.filename())
    return LoadAgentBundle(agent_path);
  }
  if (stat(properties_path.c_str(), &fileStat) == 0 &&
      fileStat.st_mtime > agent_bundle->GetLastModified()) {
    LOG_DEBUG(app_logger_,
              "Agent properties file was updated: " << agent_path.filename())
    PropertiesPtr properties = LoadProperties(properties_path);
    agent_bundle->UpdateBundle(properties, agent_path);
    agent_bundle->SetLastModified(fileStat.st_mtime);
    return agent_bundle;
  }

  return agent_bundle;
}

std::shared_ptr<s21::Properties> AgentBundleLoader::LoadProperties(
    const std::filesystem::path &properties_path) {
  PropertiesPtr properties = std::make_shared<Properties>();
  properties->Load(properties_path.string());
  return properties;
}

std::string AgentBundleLoader::FindLibraryInBundle(const fs::path &bundlePath) {
  for (const auto &entry : fs::directory_iterator(bundlePath)) {
    if (entry.is_regular_file() && entry.path().extension() == lib_extension_) {
      return entry.path().string();
    }
  }
  return "";
}

}  // namespace s21