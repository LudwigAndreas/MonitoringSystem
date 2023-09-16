//
// Created by Ludwig Andreas on 14.09.2023.
//
#include "analyzer/metric_analyzer.h"

#include <fstream>

#include "logger.h"
#include "mock/metric/mock_metric_subscriber.h"
#include "test.h"

class MetricAnalyzerTest : public ::testing::Test {
 protected:
  void SetUp() override {
    s21::diagnostic::LoggerPtr logger =
        s21::diagnostic::Logger::getLogger(log_file);

    mock_subscriber_ = std::make_shared<MockMetricSubscriber>();
    metric_analyzer_ = std::make_shared<s21::MetricAnalyzer>(log_dir, logger);
    metric_analyzer_->Subscribe(mock_subscriber_.get());
  }

  void TearDown() override {
    metric_analyzer_->Unsubscribe(mock_subscriber_.get());
    std::filesystem::remove_all(log_dir);
    std::filesystem::remove_all(log_file);
  }

  std::string log_file = "MetricAnalyzerTest.log";
  std::string log_dir = "./test_logs";
  std::shared_ptr<MockMetricSubscriber> mock_subscriber_;
  std::shared_ptr<s21::MetricAnalyzer> metric_analyzer_;
};

TEST_F(MetricAnalyzerTest, NewFileOnNewDay) {
  s21::ConfiguredMetricPtr metric = std::make_shared<s21::ConfiguredMetric>(
      "test", "test", s21::MetricCriticalValue(">=", 0), 0);
  std::string value = "value";
  std::time_t timestamp = std::time(nullptr);
  s21::MetricEvent metric_event(metric, value, timestamp);

  std::ostringstream expected_filename;
  expected_filename << log_dir << "/"
                    << std::put_time(std::localtime(&timestamp), "%m_%d_%Y")
                    << ".log";
  std::ostringstream line;
  line << "\n[" << std::put_time(std::localtime(&timestamp), "%H:%M:%S")
       << "] | " << metric->GetName() << "<" << metric->GetArgs() << ">"
       << " : " << value << " | ";
  EXPECT_CALL(*mock_subscriber_, OnNewFileOpened(expected_filename.str()))
      .Times(1);
  EXPECT_CALL(*mock_subscriber_, OnLineLogged(line.str())).Times(1);
  metric_analyzer_->Log(metric_event);
}

TEST_F(MetricAnalyzerTest, LogWithoutArgs) {
  s21::ConfiguredMetricPtr metric = std::make_shared<s21::ConfiguredMetric>(
      "test", "", s21::MetricCriticalValue(">=", 0), 0);
  std::string value = "value";
  std::time_t timestamp = std::time(nullptr);
  s21::MetricEvent metric_event(metric, value, timestamp);

  std::ostringstream expected_filename;
  expected_filename << log_dir << "/"
                    << std::put_time(std::localtime(&timestamp), "%m_%d_%Y")
                    << ".log";
  std::ostringstream line;
  line << "\n[" << std::put_time(std::localtime(&timestamp), "%H:%M:%S")
       << "] | " << metric->GetName() << " : " << value << " | ";
  EXPECT_CALL(*mock_subscriber_, OnNewFileOpened(expected_filename.str()))
      .Times(1);
  EXPECT_CALL(*mock_subscriber_, OnLineLogged(line.str())).Times(1);
  metric_analyzer_->Log(metric_event);
}

TEST_F(MetricAnalyzerTest, Unsubscribe) {
  s21::ConfiguredMetricPtr metric = std::make_shared<s21::ConfiguredMetric>(
      "test", "", s21::MetricCriticalValue(">=", 0), 0);
  std::string value = "value";
  std::time_t timestamp = std::time(nullptr);
  s21::MetricEvent metric_event(metric, value, timestamp);

  std::ostringstream expected_filename;
  expected_filename << log_dir << "/"
                    << std::put_time(std::localtime(&timestamp), "%m_%d_%Y")
                    << ".log";
  std::ostringstream line;
  line << "\n[" << std::put_time(std::localtime(&timestamp), "%H:%M:%S")
       << "] | " << metric->GetName() << " : " << value << " | ";
  metric_analyzer_->Unsubscribe(mock_subscriber_.get());
  EXPECT_CALL(*mock_subscriber_, OnNewFileOpened(expected_filename.str()))
      .Times(0);
  EXPECT_CALL(*mock_subscriber_, OnLineLogged(line.str())).Times(0);
  metric_analyzer_->Log(metric_event);
}

TEST_F(MetricAnalyzerTest, CriticalValueReached) {
  s21::ConfiguredMetricPtr metric = std::make_shared<s21::ConfiguredMetric>(
      "test", "", s21::MetricCriticalValue("==", 1), 0);
  std::string value = "1";
  std::time_t timestamp = std::time(nullptr);
  s21::MetricEvent metric_event(metric, value, timestamp);

  std::ostringstream expected_filename;
  expected_filename << log_dir << "/"
                    << std::put_time(std::localtime(&timestamp), "%m_%d_%Y")
                    << ".log";
  std::ostringstream line;
  line << "\n[" << std::put_time(std::localtime(&timestamp), "%H:%M:%S")
       << "] | " << metric->GetName() << " : " << value << " | ";
  EXPECT_CALL(*mock_subscriber_, OnNewFileOpened(expected_filename.str()))
      .Times(1);
  EXPECT_CALL(*mock_subscriber_, OnLineLogged(line.str())).Times(1);
  EXPECT_CALL(*mock_subscriber_, OnCriticalValueReached(testing::_)).Times(1);
  metric_analyzer_->Log(metric_event);
}