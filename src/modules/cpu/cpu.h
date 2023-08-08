#ifndef CPU_H
#define CPU_H

#include "../include/pstream.h"
#if __APPLE__
#include <mach/mach.h>
#include <iostream>
#endif

int		__attribute__((visibility("hidden"))) GetNumberOfProcesses();
double	__attribute__((visibility("hidden"))) GetCPULoad();


extern "C" {
	double __attribute__((visibility("default"))) cpu();
	int    __attribute__((visibility("default"))) processes();
}

#endif