#include "test.h"
#include "notifier/email/EmailAddress.h" // include the header file for your email_address class

// Define a test fixture for EmailAddress
class EmailAddressTest : public ::testing::Test {
protected:
    // Create an instance of EmailAddress for testing
    s21::EmailAddress email;

    // This function will be called before each test case
    void SetUp() override {
        // You can add setup code here if needed
        // For example, initialize the EmailAddress with a sample email
        email = s21::EmailAddress("test@example.com", "Test User");
    }
};

// Define test cases for EmailAddress
TEST_F(EmailAddressTest, DomainTest) {
    // Test the Domain method
    EXPECT_EQ(email.Domain(), "example.com");
}

TEST_F(EmailAddressTest, ConversionOperator) {
    // Test the explicit conversion operator to const char *
    const char *emailString = (const char *) email;
    EXPECT_STREQ(emailString, "test@example.com");
}

TEST_F(EmailAddressTest, OutputOperator) {
    // Test the << operator for EmailAddress
    std::ostringstream oss;
    oss << email;
    EXPECT_EQ(oss.str(), "Test User <test@example.com>");
}

// Define a test fixture for EmailAddresses
class EmailAddressesTest : public testing::Test {
protected:
    // Create an instance of EmailAddresses for testing
    s21::EmailAddresses emailAddresses;

    // This function will be called before each test case
    void SetUp() override {
        // You can add setup code here if needed
        // For example, populate the EmailAddresses vector with sample EmailAddress objects
        emailAddresses.push_back(s21::EmailAddress("user1@example.com", "User One"));
        emailAddresses.push_back(s21::EmailAddress("user2@example.com", "User Two"));
    }
};

// Define a test case for EmailAddresses
TEST_F(EmailAddressesTest, OutputOperator) {
    // Test the << operator for EmailAddresses
    std::ostringstream oss;
    oss << emailAddresses;
    EXPECT_EQ(oss.str(), "User One <user1@example.com>, User Two <user2@example.com>");
}
