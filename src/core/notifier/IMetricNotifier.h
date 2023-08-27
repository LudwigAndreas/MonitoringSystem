//
// Created by Ludwig Andreas on 27.08.2023.
//

#ifndef MONITORINGSYSTEM_IMETRICNOTIFIER_H_
#define MONITORINGSYSTEM_IMETRICNOTIFIER_H_

namespace s21 {

class IMetricNotifier {

 public:

  virtual void OnCriticalValueReached(MetricEvent event) = 0;

};

}

#endif //MONITORINGSYSTEM_IMETRICNOTIFIER_H_
