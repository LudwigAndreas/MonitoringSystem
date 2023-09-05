#ifndef CPU_H
#define CPU_H

#include <string>
#include <sstream>

#include "../include/pstream.h"
#if __linux__
#include <fstream>
#elif __APPLE__
#include <mach/mach.h>
#include <iostream>
#endif

int		__attribute__((visibility("hidden"))) GetNumberOfProcesses();
double	__attribute__((visibility("hidden"))) GetCPULoad();


std::string __attribute__((visibility("default"))) cpu();
std::string __attribute__((visibility("default"))) processes();

#endif