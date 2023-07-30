#ifndef NETWORK_H
#define NETWORK_H

#include "../pstream.h"
#include <cstdlib>
#include <fstream>
#include <filesystem>
#include <limits>
#include <sstream>
#if __APPLE__
#include <iostream>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/param.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <net/if.h>
#include <net/if_var.h>
#include <net/if_types.h>
#endif

#include "../pstream.h"
#include "../module_header.h"

double GetTotalRAM();
double GetUsedRAM();

extern "C" {
    int __attribute__((visibility("default"))) url(std::string url);

    double __attribute__((visibility("default"))) inet_throughput();
}
#endif