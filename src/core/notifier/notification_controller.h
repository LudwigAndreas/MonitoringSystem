//
// Created by Ludwig Andreas on 05.09.2023.
//

#ifndef MONITORINGSYSTEM_NOTIFICATIONCONTROLLER_H_
#define MONITORINGSYSTEM_NOTIFICATIONCONTROLLER_H_

#include "metric/i_metric_subscriber.h"
#include "metric/metric_event.h"
#include "i_message_sender.h"

namespace s21 {

class NotificationController : public IMetricSubscriber {
 public:

  NotificationController();

  virtual ~NotificationController();

  void AddNotifier(IMessageSenderPtr sender);

  void RemoveNotifier(IMessageSenderPtr sender);

  void OnCriticalValueReached(MetricEvent event) override;

  void OnLineLogged(const std::string &line) override;
  void OnNewFileOpened(const std::string &log_file) override;

 private:
  std::vector<IMessageSenderPtr> senders_;
};

}

#endif //MONITORINGSYSTEM_NOTIFICATIONCONTROLLER_H_
