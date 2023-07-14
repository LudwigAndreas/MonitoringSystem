#ifndef MEMORY_H
#define MEMORY_H

#include "../pstream.h"
#include <cstdlib>
#include <fstream>
#include <filesystem>

#include "../module_header.h"

double GetTotalRAM();
double GetUsedRAM();

extern "C" {
    double __attribute__((visibility("default"))) ram_total ();

    double __attribute__((visibility("default"))) ram ();

    double __attribute__((visibility("default"))) hard_volume ();
}
#endif