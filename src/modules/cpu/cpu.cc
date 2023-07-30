#include "cpu.h"

double GetCPULoad() {
  #if __linux__
  static size_t	working = 0;
  static size_t	total = 0;
  size_t			new_working = 0;
  size_t			new_total = 0;
  size_t			temp;
  double			load = 0;
  std::fstream	stat = std::fstream("/proc/stat", std::ios_base::in);
  std::string		name;

  stat >> name;
  if (name == "cpu") {
      for (int i = 0; i < 3; ++i) {
          stat >> temp;
          new_working += temp;
      }
      new_total = new_working;
      for (int i = 0; i < 7; ++i) {
          stat >> temp;
          new_total += temp;
      }
      load = (100 * (double)(new_working - working)) / (new_total - total);
      working = new_working;
      total = new_total;
  }
  return load;
  #elif __APPLE__
  static unsigned long long lastTotalTicks = 0;
  static unsigned long long lastIdleTicks = 0;

  unsigned long long totalTicks = 0;
  unsigned long long idleTicks = 0;

  kern_return_t kr;
  mach_msg_type_number_t count;
  host_cpu_load_info_data_t hostLoad;

  count = HOST_CPU_LOAD_INFO_COUNT;
  kr = host_statistics64(mach_host_self(),
                         HOST_CPU_LOAD_INFO,
                         (host_info64_t) &hostLoad,
                         &count);

  if (kr == KERN_SUCCESS) {
    idleTicks = hostLoad.cpu_ticks[CPU_STATE_IDLE];
    totalTicks = idleTicks + hostLoad.cpu_ticks[CPU_STATE_USER]
        + hostLoad.cpu_ticks[CPU_STATE_SYSTEM];
  }

  double percent = 0.0;
  if (lastTotalTicks != 0) {
    unsigned long long totalTicksSinceLastTime = totalTicks - lastTotalTicks;
    unsigned long long idleTicksSinceLastTime = idleTicks - lastIdleTicks;
    percent =
        (1.0 - ((double) idleTicksSinceLastTime) / totalTicksSinceLastTime)
            * 100.0;
  }

  lastTotalTicks = totalTicks;
  lastIdleTicks = idleTicks;

  return percent;
  #else
  #error "Unsupported platform"
  #endif
}

// Returns count of processes which we can get with ps aux.
// I'm not sure if using ps aux is the correct way to get all working processes.
int GetNumberOfProcesses() {
  int nop;
  redi::ipstream in("ps aux | wc -l");
  in >> nop;
  return nop - 1;
}

std::vector<std::function<int()>> GetIntegerFunctions() {
  return {GetNumberOfProcesses};
}

std::vector<std::function<double()>> GetDoubleFunctions() {
  return {GetCPULoad};
}

extern "C" {
double cpu() {
  return GetCPULoad();
}

int processes() {
  return GetNumberOfProcesses();
}
}