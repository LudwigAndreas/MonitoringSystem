#include "memory.h"
#include <iostream>

double GetTotalRAM() {
  long pages = sysconf(_SC_PHYS_PAGES);
  long page_size = sysconf(_SC_PAGE_SIZE);
//  std::cerr << "total " << pages * page_size << std::endl;
  return pages * page_size;
}

double GetFreeRAM() {
#ifdef __linux__
  // Linux implementation
    long pages = sysconf(_SC_AVPHYS_PAGES);
    long page_size = sysconf(_SC_PAGE_SIZE);
    return pages * page_size;
#elif __APPLE__
  mach_port_t host_port = mach_host_self();
  mach_msg_type_number_t count = HOST_VM_INFO_COUNT;
  vm_statistics_data_t vm_stats;

  if (host_statistics(host_port, HOST_VM_INFO, (host_info_t) &vm_stats, &count)
      != KERN_SUCCESS) {
    fprintf(stderr, "Error: Failed to fetch vm statistics.\n");
    return -1;
  }

  return (double) vm_stats.free_count * (double) vm_page_size;
#else
  // Unsupported platform
    return 0.0;
#endif
}

double GetUsedDisk() {
  const std::filesystem::space_info si = std::filesystem::space("/");
  return (1 - (double) si.free / si.capacity) * 100;
}

int GetDiskIO() {
  #if __linux__
  std::ifstream file("/proc/diskstats");
  static int io = 0;
  int old_io = io;
  unsigned int disk_id, partition_id, reads_completed, reads_merged,
      sectors_read, ms_spent_reading, writes_completed, writes_merged,
      sectors_written, ms_spent_writing, ios_in_progress, ms_spent_on_ios,
      weighted_ms_spent_on_ios;
  std::string partition_name;

  file >> disk_id >> partition_id >> partition_name >> reads_completed
       >> reads_merged >> sectors_read >> ms_spent_reading >> writes_completed
       >> writes_merged >> sectors_written >> ms_spent_writing
       >> ios_in_progress >> ms_spent_on_ios >> weighted_ms_spent_on_ios;
  file.close();
  io = reads_completed + writes_completed;
  if (old_io)
    return io - old_io;
  else
    return 0;
  #elif __APPLE__
  return 0;
  #else
  #error "Unsupported platform"
  #endif
}

extern "C" {
double ram_total() {
  return GetTotalRAM();
}

double ram() {
  double total = GetTotalRAM();
  return ((total - GetFreeRAM()) / total) * 100;
}

double hard_volume() {
  return GetUsedDisk();
}

int hard_ops() {
  return GetDiskIO();
}
}