#ifndef MEMORY_H
#define MEMORY_H

#include "../include/pstream.h"
#include <cstdlib>
#include <fstream>
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

extern "C" {
    double __attribute__((visibility("default"))) ram_total ();
    double __attribute__((visibility("default"))) ram ();
    double __attribute__((visibility("default"))) hard_volume ();
    int __attribute__((visibility("default"))) hard_ops ();
}
#endif