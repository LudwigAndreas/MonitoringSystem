#include "bonus.h"

#include <iostream>
#include <numeric>

namespace s21 {

std::vector<double> cpu::cpu_usage_ = std::vector<double>();
std::vector<double> cpu::last_usage_ = std::vector<double>();
std::vector<double> cpu::current_usage_ = std::vector<double>();

#if __linux__
void cpu::update_values() {
  std::fstream stat = std::fstream("/proc/stat", std::ios_base::in);
  std::string name;
  double cpu_value;
  // std::vector<double> cpu_usage_ = cpu_usage();
  // std::vector<double> last_usage_ = last_usage();
  // std::vector<double> current_usage_ = current_usage();

  stat >> name;
  if (name == "cpu") {
    if (current_usage_.empty())
      last_usage_ = std::vector<double>(cpu_type::SIZE, 0.);
    else
      last_usage_ = std::move(current_usage_);
    current_usage_ = {};
    for (int i = 0; i < cpu_type::SIZE; ++i) {
      stat >> cpu_value;
      current_usage_.push_back(cpu_value);
    }
    if (cpu_usage_.empty())
      cpu_usage_ = std::vector<double>(cpu_type::SIZE, 0.);
    std::transform(current_usage_.begin(), current_usage_.end(),
                   last_usage_.begin(), cpu_usage_.begin(),
                   std::minus<double>());
  }
  double sum = std::accumulate(cpu_usage_.begin(), cpu_usage_.end(), 0.) * .01;
  std::transform(cpu_usage_.begin(), cpu_usage_.end(), cpu_usage_.begin(),
                 [sum](double value) { return value / sum; });
  // cpu_usage() = cpu_usage_;
  // last_usage() = last_usage_;
  // current_usage() = current_usage_;
}

double cpu::cpu_idle_usage() { return cpu_usage_[cpu_type::idle]; }  // 4

double cpu::cpu_user_usage() { return cpu_usage_[cpu_type::user]; }  // 1

double cpu::cpu_privileged_usage() {
  return cpu_usage_[cpu_type::system];
}  // 3

double cpu::cpu_dpc_usage() { return cpu_usage_[cpu_type::nice]; }  // ??

double cpu::cpu_interrupt_usage() { return cpu_usage_[cpu_type::irq]; }  // 6

#elif __APPLE__

void cpu::update_values() {}

double cpu::cpu_idle_usage() { return 0; }  // 4

double cpu::cpu_user_usage() { return 0; }  // 1

double cpu::cpu_privileged_usage() { return 0; }  // 3

double cpu::cpu_dpc_usage() { return 0; }  // ??

double cpu::cpu_interrupt_usage() { return 0; }  // 6

#else
#error "Unsupported OS"
#endif

std::map<std::string, std::string> memory::meminfo =
    std::map<std::string, std::string>();
#if __linux__
void memory::update_values() {
  meminfo = s21::properties_reader("/proc/meminfo", ':');
}

double memory::get_value(std::string key) {
  try {
    double numeric;
    std::istringstream ss(meminfo.at(key));
    ss >> numeric;
    return numeric;
  } catch (std::out_of_range &e) {
    return -1;
  }
}

double memory::total_ram() {
  update_values();
  return get_value("MemTotal");
}

double memory::free_ram() { return get_value("MemAvailable"); }

double memory::total_swap() { return get_value("SwapTotal"); }

double memory::free_swap() { return get_value("SwapFree"); }

double memory::virtual_mem_volume() {
  return memory::total_swap() + memory::total_ram();
}

double memory::virtual_mem_free() {
  return memory::free_swap() + memory::free_ram();
}
}

#elif __APPLE__
void memory::update_values() {}

double memory::get_value(std::string key) {
  (void)key;
  return 0;
}

double memory::total_ram() { return 0; }

double memory::free_ram() { return 0; }

double memory::total_swap() { return 0; }

double memory::free_swap() { return 0; }

double memory::virtual_mem_volume() { return 0; }

double memory::virtual_mem_free() { return 0; }
}
#else
#error "Unsupported OS"
#endif

// static s21::cpu cpu;

std::string cpu_idle_usage() {
  s21::cpu::update_values();
  std::stringstream ss;
  ss << std::fixed << s21::cpu::cpu_idle_usage();
  return ss.str();
}  // 4

std::string cpu_user_usage() {
  std::stringstream ss;
  ss << std::fixed << s21::cpu::cpu_user_usage();
  return ss.str();
}  // 1

std::string cpu_privileged_usage() {
  std::stringstream ss;
  ss << std::fixed << s21::cpu::cpu_privileged_usage();
  return ss.str();
}  // 3

std::string cpu_dpc_usage() {
  std::stringstream ss;
  ss << std::fixed << s21::cpu::cpu_dpc_usage();
  return ss.str();
}  // ??

std::string cpu_interrupt_usage() {
  std::stringstream ss;
  ss << std::fixed << s21::cpu::cpu_interrupt_usage();
  return ss.str();
}  // 6

std::string total_swap() {
  std::stringstream ss;
  ss << std::fixed << s21::memory::total_swap();
  return ss.str();
}

std::string free_swap() {
  std::stringstream ss;
  ss << std::fixed << s21::memory::free_swap();
  return ss.str();
}

std::string used_swap() {
  std::stringstream ss;
  ss << std::fixed << s21::memory::total_swap() - s21::memory::free_swap();
  return ss.str();
}

std::string virtual_mem_volume() {
  std::stringstream ss;
  ss << std::fixed << s21::memory::virtual_mem_volume();
  return ss.str();
}

std::string virtual_mem_free() {
  std::stringstream ss;
  ss << std::fixed << s21::memory::virtual_mem_free();
  return ss.str();
}

std::string proc_queue_length() {
  int answer;
  redi::ipstream command("ps -e --format=\"stat\" | grep \"R\" | wc -l");
  std::stringstream ss;

  command >> answer;
  ss << std::fixed
     << answer - 1;  // Subtracting 1 because ps call also counts as runnable
  return ss.str();
}

std::string inodes() {  // df -i /
  int all_inodes, used_inodes;
  std::string line, partition;
  std::stringstream ss;
  redi::ipstream command("df -i /");

  std::getline(command, line);
  command >> partition >> all_inodes >> used_inodes;
  ss << std::fixed << used_inodes;
  return ss.str();
}

std::string hard_read_time() {
  double read, write;
  std::stringstream ss;
  redi::ipstream command(
      "iostat -d -x | tail -n 3 | head -n 1 | awk '{printf $3 \" \" $9}'");

  command >> read >> write;
  ss << std::fixed << read + write << " KB/s";
  return ss.str();
}

// grep is used to prevent systemd-coredump's stacktraces from altering error
// count that weird regex is parsing date in format "mmm dd"
std::string system_errors() {
  int error_count;
  std::stringstream ss;
  redi::ipstream command(
      "journalctl -p 3 -b -q | grep -E '[A-Z][a-z]{2} [0-9]{2}' | wc -l");

  command >> error_count;
  ss << std::fixed << error_count;
  return ss.str();
}

// Not entirely correct since it counts every opened tty and pts
std::string user_auths() {  // last -p now | ws -l
  int user_auths;
  std::stringstream ss;
  redi::ipstream command("last -p now | wc -l");

  command >> user_auths;
  ss << std::fixed << user_auths - 3;
  return ss.str();
}