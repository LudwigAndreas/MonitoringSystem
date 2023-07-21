#ifndef NETWORK_H
#define NETWORK_H

#include "../pstream.h"
#include <cstdlib>
#include <fstream>
#include <filesystem>
#include <limits>
#include <sstream>

#include "../pstream.h"
#include "../module_header.h"

#include "../../../libs21/include/s21.h"
#include "../../../libs21/test/utils/logger.inc"

double GetTotalRAM();
double GetUsedRAM();

extern "C" {
    int __attribute__((visibility("default"))) url(std::string url);

    double __attribute__((visibility("default"))) inet_throughput();
}
#endif