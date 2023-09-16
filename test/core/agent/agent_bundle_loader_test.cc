//
// Created by Ludwig Andreas on 14.09.2023.
//
#include "test.h"
#include "agent/agent_bundle_loader.h"

class AgentBundleLoaderTest : public ::testing::Test {
 protected:
  void SetUp() override {
    std::string test_module_path = "bin/test-agents/TestModule.agent";
    try {
      ASSERT_TRUE(std::filesystem::exists(test_module_path));
      ASSERT_TRUE(std::filesystem::exists(test_module_path + "/agent.properties"));
      bool found = false;
      for (const auto &entry: std::filesystem::directory_iterator(test_module_path)) {
        if (entry.is_regular_file() && IsDynamicLibrary(entry)) {
          found = true;
          break;
        }
      }
      ASSERT_TRUE(found);

      std::filesystem::path path_to_agent(test_module_path);
      auto parent_path = path_to_agent.parent_path();
      agent_path = std::filesystem::absolute(parent_path).string() + "/tmp-" + path_to_agent.filename().string();
      std::filesystem::create_directory(agent_path);
      std::filesystem::copy(test_module_path, agent_path, std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive);
    } catch (const std::filesystem::filesystem_error &e) {
      FAIL() << e.what();
    }
  }

  void TearDown() override {
    std::filesystem::remove_all(agent_path);
  }

  static bool IsDynamicLibrary(const std::filesystem::directory_entry &entry) {
    std::string extension = entry.path().extension().string();
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
    return (extension == ".dll" || extension == ".so" || extension == ".dylib");
  }

  std::string agent_path;
};

TEST_F(AgentBundleLoaderTest, LoadAgent) {
  std::shared_ptr<s21::AgentBundle> agent_bundle = s21::AgentBundleLoader::LoadAgentBundle(agent_path);
  EXPECT_NE(agent_bundle, nullptr);
  EXPECT_EQ(agent_bundle->GetName(), "test agent name");
  EXPECT_EQ(agent_bundle->GetType(), "test agent type");
  EXPECT_EQ(agent_bundle->GetAgentName(), "test agent name");
  EXPECT_EQ(agent_bundle->GetAgentType(), "test agent type");
  EXPECT_EQ(agent_bundle->GetMetrics()->size(), 3);
  EXPECT_LE(agent_bundle->GetLastModified(), std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
  EXPECT_EQ(agent_bundle->GetAgentPath(), agent_path);
}

TEST_F(AgentBundleLoaderTest, LoadBrokenAgent) {
  std::filesystem::remove_all(agent_path + "/agent.properties");
  std::shared_ptr<s21::AgentBundle> empty_agent = s21::AgentBundleLoader::LoadAgentBundle(agent_path);
  EXPECT_EQ(empty_agent, nullptr);
}

TEST_F(AgentBundleLoaderTest, UpdateAgent) {
  time_t last_modified;
  std::shared_ptr<s21::AgentBundle> agent_bundle = s21::AgentBundleLoader::LoadAgentBundle(agent_path);
  EXPECT_NE(agent_bundle, nullptr);
  EXPECT_EQ(agent_bundle->GetName(), "test agent name");
  EXPECT_EQ(agent_bundle->GetType(), "test agent type");
  EXPECT_EQ(agent_bundle->GetAgentName(), "test agent name");
  EXPECT_EQ(agent_bundle->GetAgentType(), "test agent type");
  EXPECT_EQ(agent_bundle->GetMetrics()->size(), 3);
  EXPECT_LE(agent_bundle->GetLastModified(), std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
  EXPECT_EQ(agent_bundle->GetAgentPath(), agent_path);
  last_modified = agent_bundle->GetLastModified();

  std::shared_ptr<s21::AgentBundle>
      updated_agent_bundle = s21::AgentBundleLoader::UpdateAgentBundle(agent_path, agent_bundle);
  EXPECT_NE(updated_agent_bundle, nullptr);
  EXPECT_EQ(updated_agent_bundle->GetName(), "test agent name");
  EXPECT_EQ(updated_agent_bundle->GetType(), "test agent type");
  EXPECT_EQ(updated_agent_bundle->GetAgentName(), "test agent name");
  EXPECT_EQ(updated_agent_bundle->GetAgentType(), "test agent type");
  EXPECT_EQ(updated_agent_bundle->GetMetrics()->size(), 3);
  EXPECT_LE(updated_agent_bundle->GetLastModified(),
            std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
  EXPECT_GE(updated_agent_bundle->GetLastModified(), last_modified);
  EXPECT_EQ(updated_agent_bundle->GetAgentPath(), agent_path);
}


TEST_F(AgentBundleLoaderTest, UpdateOnProperties) {
  time_t last_modified;
  std::shared_ptr<s21::AgentBundle> agent_bundle = s21::AgentBundleLoader::LoadAgentBundle(agent_path);
  EXPECT_NE(agent_bundle, nullptr);
  EXPECT_EQ(agent_bundle->GetName(), "test agent name");
  EXPECT_EQ(agent_bundle->GetType(), "test agent type");
  EXPECT_EQ(agent_bundle->GetAgentName(), "test agent name");
  EXPECT_EQ(agent_bundle->GetAgentType(), "test agent type");
  EXPECT_EQ(agent_bundle->GetMetrics()->size(), 3);
  EXPECT_LE(agent_bundle->GetLastModified(), std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
  EXPECT_EQ(agent_bundle->GetAgentPath(), agent_path);
  last_modified = agent_bundle->GetLastModified();

  std::shared_ptr<s21::AgentBundle>
      updated_agent_bundle = s21::AgentBundleLoader::UpdateAgentBundle(agent_path, agent_bundle);
  EXPECT_NE(updated_agent_bundle, nullptr);
  EXPECT_EQ(updated_agent_bundle->GetName(), "test agent name");
  EXPECT_EQ(updated_agent_bundle->GetType(), "test agent type");
  EXPECT_EQ(updated_agent_bundle->GetAgentName(), "test agent name");
  EXPECT_EQ(updated_agent_bundle->GetAgentType(), "test agent type");
  EXPECT_EQ(updated_agent_bundle->GetMetrics()->size(), 3);
  EXPECT_LE(updated_agent_bundle->GetLastModified(),
            std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
  EXPECT_GE(updated_agent_bundle->GetLastModified(), last_modified);
  EXPECT_EQ(updated_agent_bundle->GetAgentPath(), agent_path);
}
