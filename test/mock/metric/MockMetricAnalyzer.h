//
// Created by Ludwig Andreas on 14.09.2023.
//

#ifndef MONITORINGSYSTEM_TEST_MOCK_METRIC_MOCKMETRICANALYZER_H_
#define MONITORINGSYSTEM_TEST_MOCK_METRIC_MOCKMETRICANALYZER_H_

#include "test.h"
#include "analyzer/MetricAnalyzer.h"
#include "metric/IMetricSubscriber.h"

class MockMetricAnalyzer : public s21::MetricAnalyzer {
  public:

  MOCK_METHOD(void, Log, (s21::MetricEvent& event));

  MOCK_METHOD(void, Subscribe, (s21::IMetricSubscriber *notifier));

  MOCK_METHOD(void, Unsubscribe, (s21::IMetricSubscriber *notifier));

};

#endif //MONITORINGSYSTEM_TEST_MOCK_METRIC_MOCKMETRICANALYZER_H_
