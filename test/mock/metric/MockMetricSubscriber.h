//
// Created by Ludwig Andreas on 14.09.2023.
//

#ifndef MONITORINGSYSTEM_TEST_MOCK_CONFIG_MOCKMETRICSUBSCRIBER_H_
#define MONITORINGSYSTEM_TEST_MOCK_CONFIG_MOCKMETRICSUBSCRIBER_H_

#include "test.h"
#include "metric/IMetricSubscriber.h"

class MockMetricSubscriber : public s21::IMetricSubscriber {
 public:
  MOCK_METHOD1(OnCriticalValueReached, void(s21::MetricEvent event));
  MOCK_METHOD1(OnLineLogged, void(const std::string& line));
  MOCK_METHOD1(OnNewFileOpened, void(const std::string& log_file));
};

#endif //MONITORINGSYSTEM_TEST_MOCK_CONFIG_MOCKMETRICSUBSCRIBER_H_
