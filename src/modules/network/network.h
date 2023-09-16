#ifndef NETWORK_H
#define NETWORK_H

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <limits>
#include <sstream>
#if __APPLE__
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <net/if_types.h>
#include <net/if_var.h>
#include <netinet/in.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/time.h>

#include <iostream>
#endif

// #include "modules/include/pstream.h"
// #include "modules/include/module_header.h"

#include "../include/pstream.h"

std::string __attribute__((visibility("default"))) url(std::string url);
std::string __attribute__((visibility("default"))) inet_throughput();

#endif