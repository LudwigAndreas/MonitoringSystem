#ifndef BONUS_H
#define BONUS_H

#include "../pstream.h"
#include <cstdlib>
#include <fstream>
#include <filesystem>
#include <limits>
#include <sstream>

#include "../pstream.h"
#include "../module_header.h"

#include "../../../lib/libs21/include/s21.h"
// #include "../../../libs21/test/utils/analyzer.inc"

extern "C" {
                                                                            // /proc/stat
    double __attribute__((visibility("default"))) cpu_idle_usage();
    double __attribute__((visibility("default"))) cpu_user_usage();
    double __attribute__((visibility("default"))) cpu_privileged_usage();
    double __attribute__((visibility("default"))) cpu_dpc_usage();
    double __attribute__((visibility("default"))) cpu_interrupt_usage();

    double __attribute__((visibility("default"))) total_swap();
    double __attribute__((visibility("default"))) used_swap();

    int __attribute__((visibility("default"))) proc_queue_length();
        
    double __attribute__((visibility("default"))) virtual_mem_volume();
    double __attribute__((visibility("default"))) virtual_mem_free();

    int __attribute__((visibility("default"))) inodes();

    double __attribute__((visibility("default"))) hard_read_time(); // not implemented

    int __attribute__((visibility("default"))) system_errors();

    int __attribute__((visibility("default"))) user_auths();

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