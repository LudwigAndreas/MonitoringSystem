#include <gtest/gtest.h>
#include "notifier/telegram/TelegramUserRepository.h" // include the header file for your telegram_user_repository class

// Define a test fixture
class TelegramUserRepositoryTest : public testing::Test {
protected:
    // Create an instance of the TelegramUserRepository for testing
    s21::TelegramUserRepository repository;

    // This function will be called before each test case
    void SetUp() override {
        // You can add setup code here if needed
        // For example, add users to the repository
        repository.AddUser("user1", "12345");
        repository.AddUser("user2", "67890");
    }
};

// Define test cases
TEST_F(TelegramUserRepositoryTest, GetUser) {
    // Test the GetUser method
    EXPECT_EQ(repository.GetUser("user1"), "12345");
    EXPECT_EQ(repository.GetUser("user2"), "67890");
    EXPECT_EQ(repository.GetUser("nonexistent_user"), "");
}

TEST_F(TelegramUserRepositoryTest, AddUser) {
    // Test the AddUser method
    repository.AddUser("new_user", "54321");
    EXPECT_EQ(repository.GetUser("new_user"), "54321");
}
