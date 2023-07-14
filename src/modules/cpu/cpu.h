#ifndef CPU_H
#define CPU_H

#include "../module_header.h"
#include "../pstream.h"

int		__attribute__((visibility("default"))) GetNumberOfProcesses();
double	__attribute__((visibility("default"))) GetCPULoad();

#endif