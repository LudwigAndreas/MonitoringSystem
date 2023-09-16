//
// Created by Ludwig Andreas on 14.09.2023.
//

#ifndef MONITORINGSYSTEM_TEST_MOCK_CONFIG_MOCKMETRICSUBSCRIBER_H_
#define MONITORINGSYSTEM_TEST_MOCK_CONFIG_MOCKMETRICSUBSCRIBER_H_

#include "test.h"
#include "metric/imetric_subscriber.h"

class MockMetricSubscriber : public s21::IMetricSubscriber {
 public:
  MOCK_METHOD(void, OnCriticalValueReached, (s21::MetricEvent));
  MOCK_METHOD(void, OnLineLogged, (const std::string& line));
  MOCK_METHOD(void, OnNewFileOpened, (const std::string& log_file));
};

#endif //MONITORINGSYSTEM_TEST_MOCK_CONFIG_MOCKMETRICSUBSCRIBER_H_
