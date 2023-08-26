//
// Created by Ludwig Andreas on 24.08.2023.
//

#ifndef MONITORINGSYSTEM_SRC_CORE_AGENT_AGENTBUNDLELOADER_H_
#define MONITORINGSYSTEM_SRC_CORE_AGENT_AGENTBUNDLELOADER_H_

#include "agent_bundle.h"

namespace s21 {

namespace fs = std::filesystem;

class AgentBundleLoader {

 public:

  /**
   * Loads an agent bundle from a given path.
   *
   * @param agent_path path to the agent bundle
   *
   * @return std::shared_ptr of AgentBundle on success and nullptr on failure
   * */
  static std::shared_ptr<AgentBundle> LoadAgentBundle(const std::filesystem::path &agent_path);

 private:

  /**
   * A helper method for getting an agent object from a dynamic library.
   *
   * @param lib pointer to an open dynamic library that has a lib_entry_point method
   *
   * @return std::shared_ptr of Agent on success and nullptr on failure
   * */
  static std::shared_ptr<s21::Agent> Instantiate(DynamicLib lib);

  /**
   * A helper method for loading the properties of an agent from a file.
   *
   * @param properties_path path to the properties file
   *
   * @return std::shared_ptr of Properties on success and nullptr on failure
   * */
  static std::shared_ptr<s21::Properties> LoadProperties(const std::filesystem::path& properties_path);


  /**
   * A helper method for finding the library in the bundle.
   *
   * @param bundlePath path to the bundle
   *
   * @return path to the library on success and empty string on failure
   * */
  static std::string FindLibraryInBundle(const fs::path& bundlePath);


  const std::string agent_extension_ = ".agent";
  static const std::string lib_extension_;
  static const std::string lib_entry_point_;
  static const std::string agent_properties_path_;
};

}

#endif //MONITORINGSYSTEM_SRC_CORE_AGENT_AGENTBUNDLELOADER_H_
