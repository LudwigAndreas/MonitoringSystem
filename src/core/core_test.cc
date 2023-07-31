//
// Created by Ludwig Andreas on 30.07.2023.
//

#include <iostream>
#include <vector>
#include <dlfcn.h>

#include "../modules/include/Metric.h"
#include "../modules/include/Agent.h"

typedef void* dynamic_lib_handle;

static s21::Agent* instantiate(const dynamic_lib_handle handle) {

  if (handle == nullptr) return nullptr;

  void *maker = dlsym(handle , "get_agent");

  if (maker == nullptr) return nullptr;

  typedef s21::Agent* (*fptr)();
  fptr func = reinterpret_cast<fptr>(reinterpret_cast<void*>(maker));

  return func();
}

int main() {
  void *dumb_lib = dlopen("/Users/landreas-pc/42Course/MonitoringSystem/lib/libDumbModule.dylib", RTLD_LAZY);
  if (!dumb_lib) {
    std::cerr << dlerror() << std::endl;
    return 1;
  }
  s21::Agent* agent = instantiate(dumb_lib);
  if (!agent) {
    std::cerr << dlerror() << std::endl;
    return 1;
  }
  auto metrics = agent->GetMetrics();
  for (const auto& metric : metrics) {
    std::cout << metric.GetName() << std::endl;
    std::cout << metric.run() << std::endl;
  }

}