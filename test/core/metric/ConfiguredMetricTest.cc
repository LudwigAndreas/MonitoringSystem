#include "test.h"
#include "metric/ConfiguredMetric.h"
#include "metric/MetricCriticalValue.h"

using namespace s21;

// Define test fixtures if needed
class ConfiguredMetricTest : public ::testing::Test {
 protected:
  // You can set up common test data and objects here
  ConfiguredMetric metric;

  // This function will be called before each test case
  void SetUp() override {
    // Initialize the ConfiguredMetric instance for testing
    // You can provide appropriate initial values as needed
    metric = ConfiguredMetric("TestMetric", "arg1 arg2", MetricCriticalValue("==", 0), 60);
  }

  void TearDown() override {
    // Free any memory allocated in SetUp()
  }
};

// Define test cases using the ConfiguredMetricTest fixture
TEST_F(ConfiguredMetricTest, GetName) {
  // Test the GetName() method
  metric.SetName("TestMetric");
  EXPECT_EQ(metric.GetName(), "TestMetric");
}

TEST_F(ConfiguredMetricTest, SetArgs) {
  // Test the SetArgs() method
  metric.SetArgs("arg1 arg2");
  EXPECT_EQ(metric.GetArgs(), "arg1 arg2");
}