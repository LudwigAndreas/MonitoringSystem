#ifndef BONUS_H
#define BONUS_H

#include "../include/pstream.h"
#include <cstdlib>
#include <fstream>
#include <filesystem>
#include <limits>
#include <sstream>
#include <vector>

#include "../../../third_party/libs21/include/s21.h"
// #include "../../../libs21/test/utils/analyzer.inc"

namespace s21 {
class cpu {  
  enum cpu_type {
      user,
      nice,
      system,
      idle,
      iowait,
      irq,
      softirq,
      steal,
      guest,
      guest_nice,
      SIZE
  };

  static std::vector<double> cpu_usage_;
  static std::vector<double> last_usage_;
  static std::vector<double> current_usage_;
  static int value;

  // static std::vector<double> cpu_usage() {
  //   static std::vector<double> _ = std::vector<double>();
  //   return _;
  // }

  // static std::vector<double> last_usage() {
  //   static std::vector<double> _ = std::vector<double>();
  //   return _;
  // }

  // static std::vector<double> current_usage() {
  //   static std::vector<double> _ = std::vector<double>();
  //   return _;
  // }

  public:
  static void update_values();

  static double cpu_idle_usage();

  static double cpu_user_usage();

  static double cpu_privileged_usage();

  static double cpu_dpc_usage();

  static double cpu_interrupt_usage();
};

class memory {
  static std::map<std::string, std::string> meminfo;

  public:
  static void update_values();

  static double get_value(std::string key);

  static double total_ram();

  static double free_ram();

  static double total_swap();

  static double free_swap();

  static double virtual_mem_volume();

  static double virtual_mem_free();
};

}
                                                                        // /proc/stat
std::string __attribute__((visibility("default"))) cpu_idle_usage();
std::string __attribute__((visibility("default"))) cpu_user_usage();
std::string __attribute__((visibility("default"))) cpu_privileged_usage();
std::string __attribute__((visibility("default"))) cpu_dpc_usage();
std::string __attribute__((visibility("default"))) cpu_interrupt_usage();

std::string __attribute__((visibility("default"))) total_swap();
std::string __attribute__((visibility("default"))) used_swap();
std::string __attribute__((visibility("default"))) free_swap();

std::string __attribute__((visibility("default"))) proc_queue_length();
  
std::string __attribute__((visibility("default"))) virtual_mem_volume();
std::string __attribute__((visibility("default"))) virtual_mem_free();

std::string __attribute__((visibility("default"))) inodes();

std::string __attribute__((visibility("default"))) hard_read_time(); // not implemented

std::string __attribute__((visibility("default"))) system_errors();

std::string __attribute__((visibility("default"))) user_auths();

// `[double]` загрузка процессора по уровням привилегий: `idle`, `user`, `priveleged`, `dpc`, `interrupt` (для каждого уровня выделить процент) (`cpu_idle_usage`, `cpu_user_usage`, ...)
// `[double]` общий объем свопа/swap'а (`total_swap`)
// `[double]` используемый объем свопа/swap'а (`used_swap`) 
// `[int]` количество готовых к выполнению процессов в очереди (при необходимости прочитать про состояния процессов в *Unix*) (`proc_queue_length`)  // ps -e --format="pid cmd stat"
// `[double]` подсчет общей и свободной виртуальной памяти (`virtual_mem_volume`, `virtual_mem_free`)
// `[int]` общее число inode'ов (`inodes`)
// `[double]` среднее время чтения с жесткого диска (`hard_read_time`)
// `[int]` число ошибок из системного журнала (`system_errors`)
// `[int]` количество авторизаций пользователей (`user_auths`) 

#endif