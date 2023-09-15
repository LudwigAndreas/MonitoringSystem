//
// Created by Ludwig Andreas on 14.09.2023.
//
#include "test.h"
#include "core/Core.h"

//TODO add observer tests
class CoreTest : public ::testing::Test {
 protected:
  void SetUp() override {
    core_ = std::make_shared<s21::monitor::Core>();
  }

  void TearDown() override {
  }

  std::shared_ptr<s21::monitor::Core> core_;
};

TEST_F(CoreTest, SubscribesToMetric) {

}

TEST_F(CoreTest, SubscribeToAgent) {

}

TEST_F(CoreTest, UnsubscribeFromAgent) {

}

TEST_F(CoreTest, UnsubscribeFromMetric) {

}

TEST(CoreConstructorTest, Singleton) {
  auto core1 = s21::monitor::Core();
  EXPECT_EXIT(s21::monitor::Core(), ::testing::ExitedWithCode(1), ".*");
}

