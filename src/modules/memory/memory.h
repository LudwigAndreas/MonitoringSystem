#ifndef MEMORY_H
#define MEMORY_H

#include "../include/pstream.h"
#include <cstdlib>
#include <fstream>
#include <string>
#include <filesystem>

#if __APPLE__
#include <stdlib.h> // For malloc on macOS
#include <mach/mach.h> // For vm_statistics on macOS

#include <iostream>
#include <IOKit/IOKitLib.h>
#include <IOKit/storage/IOMedia.h>
#endif

double GetTotalRAM();
double GetUsedRAM();

std::string __attribute__((visibility("default"))) ram_total ();
std::string __attribute__((visibility("default"))) ram ();
std::string __attribute__((visibility("default"))) hard_volume ();
std::string __attribute__((visibility("default"))) hard_ops ();
std::string __attribute__((visibility("default"))) hard_throughput ();

#endif