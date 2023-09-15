//
// Created by Ludwig Andreas on 10.09.2023.
//

#include "test.h"

#include <iostream>
#include <fstream>
#include <ctime>
#include <future>

#include "agent/AgentManager.h"
#include "mock/agent/MockAgentSubscriber.h"

//TODO rewrite test to check another thread calls the subscriber
class AgentManagerTest : public ::testing::Test {
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
      agents_path = std::filesystem::absolute(parent_path).string() + "-tmp/";
      test_agent = agents_path + path_to_agent.filename().string();
      std::filesystem::create_directory(agents_path);
      std::filesystem::copy(parent_path,
                            agents_path,
                            std::filesystem::copy_options::overwrite_existing
                                | std::filesystem::copy_options::recursive);
    } catch (const std::filesystem::filesystem_error &e) {
      FAIL() << e.what();
    }
    app_logger_ = s21::diagnostic::Logger::getRootLogger();
    agent_manager_ = std::make_unique<s21::AgentManager>(agents_path, 0);

    mock_subscriber_ = std::make_shared<MockAgentSubscriber>();
    agent_manager_->Subscribe(mock_subscriber_.get());
  }

  void TearDown() override {
    // Stop monitoring thread if it's running
    if (agent_manager_->IsMonitoring()) {
      agent_manager_->StopMonitoring();
    }
    std::filesystem::remove_all("./mock_agents/");
  }

  static bool IsDynamicLibrary(const std::filesystem::directory_entry &entry) {
    std::string extension = entry.path().extension().string();
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
    return (extension == ".dll" || extension == ".so" || extension == ".dylib");
  }

  std::string test_agent;
  std::string agents_path;
  std::shared_ptr<s21::diagnostic::Logger> app_logger_;
  std::unique_ptr<s21::AgentManager> agent_manager_;
  std::shared_ptr<MockAgentSubscriber> mock_subscriber_;
};

TEST_F(AgentManagerTest, StartAndStopMonitoring) {
  agent_manager_->StartMonitoring();

  EXPECT_CALL(*mock_subscriber_, OnAgentAdded(::testing::_)).Times(testing::AtLeast(1));

  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  EXPECT_TRUE(agent_manager_->IsMonitoring());

  agent_manager_->StopMonitoring();

  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  EXPECT_FALSE(agent_manager_->IsMonitoring());
}

TEST_F(AgentManagerTest, AgentAddedNotification) {

  auto& mock_subscriber = mock_subscriber_;

  EXPECT_CALL(*mock_subscriber_, OnAgentAdded(::testing::_)).WillOnce([&mtx, &cv] {
    std::unique_lock<std::mutex> lck(mtx);
    cv.notify_one();
  });


  cv.wait_for(lck, std::chrono::milliseconds(100), [&] {
    agent_manager_->StartMonitoring();
    return !agent_manager_->GetAgentList()->empty();
  });

  agent_manager_->StopMonitoring();
}

//TEST_F(AgentManagerTest, AgentUpdatedNotification) {
//
//  std::mutex mtx;
//  std::unique_lock<std::mutex> lck(mtx);
//  std::condition_variable cv;
//
//  EXPECT_CALL(*mock_subscriber_, OnAgentAdded(::testing::_)).WillOnce([&mtx, &cv] {
//    std::unique_lock<std::mutex> lck(mtx);
//    cv.notify_one();
//  });
//
//
//  cv.wait_for(lck, std::chrono::milliseconds(100), [&] {
//    agent_manager_->StartMonitoring();
//    return !agent_manager_->GetAgentList()->empty();
//  });
//
//  std::ofstream properties(test_agent + "/agent.properties", std::ios::out);
//
//
//  properties << "# comment" << std::endl;
//  properties.close();
//  if (!properties) {
//    FAIL() << "Could not open file!";
//  }
//
//  EXPECT_CALL(*mock_subscriber_, OnAgentUpdated(::testing::_)).WillOnce([&mtx, &cv] {
//    std::unique_lock<std::mutex> lck(mtx);
//    cv.notify_one();
//  });
//
//  cv.wait_for(lck, std::chrono::milliseconds(100), [&] {
//    return agent_manager_->GetAgentList()->empty();
//  });
////  EXPECT_CALL(*mock_subscriber_, OnAgentUpdated(::testing::_)).Times(testing::AtLeast(1));
//
//  agent_manager_->StopMonitoring();
//}
//
//TEST_F(AgentManagerTest, AgentRemovedNotification) {
//
//  std::mutex mtx;
////  auto& agent_manager = agent_manager_;
//  EXPECT_CALL(*mock_subscriber_, OnAgentAdded(::testing::_)).WillOnce([&mtx] {
//    mtx.unlock();
//  });
//  agent_manager_->StartMonitoring();
//  mtx.lock();
//
////  EXPECT_CALL(*mock_subscriber_, OnAgentAdded(::testing::_));
//  agent_manager_->StopMonitoring();
//}

TEST_F(AgentManagerTest, DeleteAgentTest) {

  std::filesystem::path tempDir = "./temp_agent_test";
  std::filesystem::create_directory(tempDir);

  s21::AgentBundlePtr mockAgent = std::make_shared<s21::AgentBundle>();
  mockAgent->SetName("MockAgent");
  mockAgent->SetAgentPath(tempDir);
  agent_manager_->GetAgentList()->emplace("MockAgent", mockAgent);

  agent_manager_->StartMonitoring();
  EXPECT_CALL(*mock_subscriber_, OnAgentAdded(::testing::_)).Times(testing::AtLeast(1));

  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  agent_manager_->DeleteAgent(mockAgent);

  EXPECT_TRUE(std::filesystem::exists(tempDir));

  agent_manager_->StopMonitoring();

  std::filesystem::remove_all(tempDir);
}

TEST_F(AgentManagerTest, GetAgentList) {

  agent_manager_->StartMonitoring();

  EXPECT_CALL(*mock_subscriber_, OnAgentAdded(::testing::_)).Times(testing::AtLeast(1));

  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  std::map<std::string, s21::AgentBundlePtr> *agent_list = agent_manager_->GetAgentList();

  EXPECT_FALSE(agent_list->empty());

  agent_manager_->StopMonitoring();
}


TEST_F(AgentManagerTest, UnsubscribedAgentAddedNotification) {

  agent_manager_->StartMonitoring();

  agent_manager_->Unsubscribe(mock_subscriber_.get());

  EXPECT_CALL(*mock_subscriber_, OnAgentAdded(::testing::_)).Times(testing::AtLeast(0));

  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  std::map<std::string, s21::AgentBundlePtr> *agent_list = agent_manager_->GetAgentList();

  EXPECT_FALSE(agent_list->empty());

  agent_manager_->StopMonitoring();
}
