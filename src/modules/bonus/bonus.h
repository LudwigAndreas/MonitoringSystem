#ifndef BONUS_H
#define BONUS_H

#include "../include/pstream.h"
#include <cstdlib>
#include <fstream>
#include <filesystem>
#include <limits>
#include <sstream>

#include "../../../third_party/libs21/include/s21.h"
// #include "../../../libs21/test/utils/analyzer.inc"

extern "C" {
                                                                          // /proc/stat
  std::string __attribute__((visibility("default"))) cpu_idle_usage();
  std::string __attribute__((visibility("default"))) cpu_user_usage();
  std::string __attribute__((visibility("default"))) cpu_privileged_usage();
  std::string __attribute__((visibility("default"))) cpu_dpc_usage();
  std::string __attribute__((visibility("default"))) cpu_interrupt_usage();

  std::string __attribute__((visibility("default"))) total_swap();
  std::string __attribute__((visibility("default"))) used_swap();

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

}
#endif