//
// Created by Ludwig Andreas on 10.09.2023.
//

#include "test.h"

#include <iostream>
#include <fstream>
#include <ctime>
#include <future>

#include "agent/agent_manager.h"
#include "mock/agent/mock_agent_subscriber.h"

// Unable to write tests for async class without absl::synchronization and absl::Notification

// class AgentManagerTest : public ::testing::Test {
//  protected:
//   void SetUp() override {
//     std::string test_module_path = "bin/test-agents/TestModule.agent";
//     try {
//       ASSERT_TRUE(std::filesystem::exists(test_module_path));
//       ASSERT_TRUE(std::filesystem::exists(test_module_path + "/agent.properties"));
//       bool found = false;
//       for (const auto &entry: std::filesystem::directory_iterator(test_module_path)) {
//         if (entry.is_regular_file() && IsDynamicLibrary(entry)) {
//           found = true;
//           break;
//         }
//       }
//       ASSERT_TRUE(found);

//       std::filesystem::path path_to_agent(test_module_path);

//       auto parent_path = path_to_agent.parent_path();
//       agents_path = std::filesystem::absolute(parent_path).string() + "-tmp/";
//       test_agent = agents_path + path_to_agent.filename().string();
//       std::filesystem::create_directory(agents_path);
//       std::filesystem::copy(parent_path,
//                             agents_path,
//                             std::filesystem::copy_options::overwrite_existing
//                                 | std::filesystem::copy_options::recursive);
//     } catch (const std::filesystem::filesystem_error &e) {
//       FAIL() << e.what();
//     }
//     app_logger_ = s21::diagnostic::Logger::getRootLogger();
//     agent_manager_ = std::make_unique<s21::AgentManager>(agents_path, 0);

//     mock_subscriber_ = std::make_shared<MockAgentSubscriber>();
//     agent_manager_->Subscribe(mock_subscriber_.get());
//   }

//   void TearDown() override {
//     // Stop monitoring thread if it's running
//     if (agent_manager_.get()) {
//       if (agent_manager_->IsMonitoring())
//         agent_manager_->StopMonitoring();
//       agent_manager_->Unsubscribe(mock_subscriber_.get());
//     }
//     std::filesystem::remove_all(agents_path);
//   }

//   static bool IsDynamicLibrary(const std::filesystem::directory_entry &entry) {
//     std::string extension = entry.path().extension().string();
//     std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
//     return (extension == ".dll" || extension == ".so" || extension == ".dylib");
//   }

//   std::string test_agent;
//   std::string agents_path;
//   std::shared_ptr<s21::diagnostic::Logger> app_logger_;
//   std::unique_ptr<s21::AgentManager> agent_manager_;
//   std::shared_ptr<MockAgentSubscriber> mock_subscriber_;
// };

// TEST_F(AgentManagerTest, StartAndStopMonitoring) {
//   agent_manager_->StartMonitoring();

//   // EXPECT_CALL(*mock_subscriber_, OnAgentAdded(::testing::_)).Times(testing::AtLeast(1));

//   std::this_thread::sleep_for(std::chrono::milliseconds(100));
//   EXPECT_TRUE(agent_manager_->IsMonitoring());

//   agent_manager_->StopMonitoring();

//   std::this_thread::sleep_for(std::chrono::milliseconds(100));

//   EXPECT_FALSE(agent_manager_->IsMonitoring());
// }

// TEST_F(AgentManagerTest, GetAgentList) {

//   agent_manager_->StartMonitoring();

//   // EXPECT_CALL(*mock_subscriber_, OnAgentAdded(::testing::_)).Times(testing::AtLeast(1));

//   std::this_thread::sleep_for(std::chrono::milliseconds(100));
//   std::map<std::string, s21::AgentBundlePtr> *agent_list = agent_manager_->GetAgentList();

//   EXPECT_FALSE(agent_list->empty());

//   agent_manager_->StopMonitoring();
// }


// TEST_F(AgentManagerTest, UnsubscribedAgentAddedNotification) {

//   agent_manager_->StartMonitoring();

//   agent_manager_->Unsubscribe(mock_subscriber_.get());

//   EXPECT_CALL(*mock_subscriber_, OnAgentAdded(::testing::_)).Times(testing::AtLeast(0));

//   std::this_thread::sleep_for(std::chrono::milliseconds(100));
//   std::map<std::string, s21::AgentBundlePtr> *agent_list = agent_manager_->GetAgentList();

//   EXPECT_FALSE(agent_list->empty());

//   agent_manager_->StopMonitoring();
// }
