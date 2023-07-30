#ifndef CPU_H
#define CPU_H

#include "../module_header.h"
#include "../pstream.h"
#if __APPLE__
#include <mach/mach.h>
#include <iostream>
#endif

int		__attribute__((visibility("default"))) GetNumberOfProcesses();
double	__attribute__((visibility("default"))) GetCPULoad();


extern "C" {
	double __attribute__((visibility("default"))) cpu();
	int    __attribute__((visibility("default"))) processes();
}

#endif