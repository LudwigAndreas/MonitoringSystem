#ifndef MODULE_HEADER_H
#define MODULE_HEADER_H

#include <cstdlib>
#include <fstream>
#include <functional>
#include <vector>

std::vector<std::function<int ()>>		__attribute__((visibility("default"))) GetIntegerFunctions();
std::vector<std::function<double ()>>	__attribute__((visibility("default"))) GetDoubleFunctions();

#endif