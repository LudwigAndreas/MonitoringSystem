#ifndef MEMORY_H
#define MEMORY_H

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string>

#include "../include/pstream.h"

#if __APPLE__
#include <IOKit/IOKitLib.h>
#include <IOKit/storage/IOMedia.h>
#include <mach/mach.h>  // For vm_statistics on macOS
#include <stdlib.h>     // For malloc on macOS

#include <iostream>
#endif

double GetTotalRAM();
double GetUsedRAM();

std::string __attribute__((visibility("default"))) ram_total();
std::string __attribute__((visibility("default"))) ram();
std::string __attribute__((visibility("default"))) hard_volume();
std::string __attribute__((visibility("default"))) hard_ops();
std::string __attribute__((visibility("default"))) hard_throughput();

#endif