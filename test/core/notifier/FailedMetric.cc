#include <gtest/gtest.h>
#include "../../../src/core/notifier/FailedMetric.h" // Include the header file for your s21::FailedMetric class

// Define a test fixture
class FailedMetricTest : public testing::Test {
protected:
    // You can add setup code here if needed
    // For example, create instances of s21::FailedMetric to test
    s21::FailedMetric failedMetric1;
    s21::FailedMetric failedMetric2;

    // This function will be called before each test case
    void SetUp() override {
        // Initialize the s21::FailedMetric instances as needed for testing
        failedMetric1 = s21::FailedMetric("Metric1", "Critical1", "Value1", "Date1");
        failedMetric2 = s21::FailedMetric("Metric2", "Critical2", "Value2", "Date2");
    }
};

// Define test cases
TEST_F(FailedMetricTest, ConstructorWithValues) {
    // Test the constructor that takes individual values as arguments
    EXPECT_EQ(failedMetric1.metric_name, "Metric1");
    EXPECT_EQ(failedMetric1.critical_value, "Critical1");
    EXPECT_EQ(failedMetric1.value, "Value1");
    EXPECT_EQ(failedMetric1.date, "Date1");
}

TEST_F(FailedMetricTest, ConstructorWithMetricEvent) {
    auto ptr = std::make_shared<s21::ConfiguredMetric>(nullptr);
    std::string str = "null";
    auto tim = time(NULL);
	s21::MetricEvent metricEvent(
		ptr,
		str,
		tim
	);

    s21::FailedMetric failedMetric(metricEvent);

    EXPECT_EQ(failedMetric.metric_name, metricEvent.GetMetric()->GetName());
    EXPECT_EQ(failedMetric.value, metricEvent.GetValue());
    EXPECT_EQ(failedMetric.date, asctime(gmtime(&tim)));
}
