#include "memory.h"

#include <iostream>
#include <sstream>

double GetTotalRAM() {
  long pages = sysconf(_SC_PHYS_PAGES);
  long page_size = sysconf(_SC_PAGE_SIZE);
  //  std::cerr << "total " << pages * page_size << std::endl;
  return pages * page_size;
}

#ifdef __linux__
double GetFreeRAM() {
  long pages = sysconf(_SC_AVPHYS_PAGES);
  long page_size = sysconf(_SC_PAGE_SIZE);
  return pages * page_size;
}
#elif __APPLE__
double GetFreeRAM() {
  mach_port_t host_port = mach_host_self();
  mach_msg_type_number_t count = HOST_VM_INFO_COUNT;
  vm_statistics_data_t vm_stats;

  if (host_statistics(host_port, HOST_VM_INFO, (host_info_t)&vm_stats,
                      &count) != KERN_SUCCESS) {
    fprintf(stderr, "Error: Failed to fetch vm statistics.\n");
    return -1;
  }

  return (double)vm_stats.free_count * (double)vm_page_size;
}
#else
double GetFreeRAM() {
  // Unsupported platform
  return 0.0;
}
#endif

double GetUsedDisk() {
  const std::filesystem::space_info si = std::filesystem::space("/");
  return (1 - (double)si.free / si.capacity) * 100;
}

#if __linux__
int GetDiskIO() {
  std::ifstream file("/proc/diskstats");
  static int io = 0;
  int old_io = io;
  unsigned int disk_id, partition_id, reads_completed, reads_merged,
      sectors_read, ms_spent_reading, writes_completed, writes_merged,
      sectors_written, ms_spent_writing, ios_in_progress, ms_spent_on_ios,
      weighted_ms_spent_on_ios;
  std::string partition_name;

  file >> disk_id >> partition_id >> partition_name >> reads_completed >>
      reads_merged >> sectors_read >> ms_spent_reading >> writes_completed >>
      writes_merged >> sectors_written >> ms_spent_writing >> ios_in_progress >>
      ms_spent_on_ios >> weighted_ms_spent_on_ios;
  file.close();
  io = reads_completed + writes_completed;
  if (old_io)
    return io - old_io;
  else
    return 0;
}
#elif __APPLE__
int GetDiskIO() {
  int tps;
  redi::ipstream in("iostat | awk 'NR > 2 { print $2 + $5 }'");
  in >> tps;
  return tps;
}
#else
int GetDiskIO() { return 0; }
#endif

std::string ram_total() {
  std::stringstream ss;
  ss << std::fixed << GetTotalRAM();
  return ss.str();
}

std::string ram() {
  std::stringstream ss;
  double total = GetTotalRAM();
  ss << std::fixed << ((total - GetFreeRAM()) / total) * 100;
  return ss.str();
}

std::string hard_volume() {
  std::stringstream ss;
  ss << std::fixed << GetUsedDisk();
  return ss.str();
}

std::string hard_ops() {
  std::stringstream ss;
  ss << std::fixed << GetDiskIO();
  return ss.str();
}

std::string hard_throughput() {
  std::string output;
  redi::ipstream ip(
      "part=$(lsblk | head -n 2 | tail -n 1 | awk '{printf $1}'); dd "
      "if=/dev/$part of=/dev/null bs=1M count=100 2>&1 >/dev/null | tail -n 1 "
      "| awk \'{printf $10\" \" $11}\'");
  std::getline(ip, output);
  return output;
}