//
// Created by Ludwig Andreas on 14.09.2023.
//
#include "test.h"

#include <fstream>

#include "analyzer/MetricAnalyzer.h"
#include "Logger.h"
#include "mock/metric/MockMetricSubscriber.h"

//TODO add observer handling tests
class MetricAnalyzerTest : public ::testing::Test {
 protected:
  void SetUp() override {
    s21::diagnostic::LoggerPtr logger = s21::diagnostic::Logger::getLogger(log_file);

    logger->AddOutputStream(new std::ofstream("MetricAnalyzerTest.log", std::ios::out | std::ios::app), true, s21::diagnostic::LogLevel::Trace);


    metric_analyzer_ = std::make_shared<s21::MetricAnalyzer>(log_dir, logger);
  }

  void TearDown() override {

  }

  std::string log_file = "MetricAnalyzerTest.log";
  std::string log_dir = "./test_logs/";
  std::shared_ptr<s21::MetricAnalyzer> metric_analyzer_;
};

//TEST_F(MetricAnalyzerTest, )