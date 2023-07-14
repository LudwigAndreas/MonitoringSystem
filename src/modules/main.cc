#include "module_header.h"

#include <iostream>
#include <functional>
#include <unistd.h>
#include <vector>
#include <dlfcn.h>
#include <cstdlib>

typedef std::vector<std::function<double ()>> dvec;
typedef std::vector<std::function<int ()>>	  ivec;

typedef dvec (*dfunc)();

struct Library {
	std::string name;
	void		*handle;
	dvec		dfuncs;
	ivec		ifuncs;
};


// Library OpenLibrary

int main() {
	void *handle = dlopen("/home/andref/Documents/s21/MonitoringSystem/src/modules/libmscpu.so", RTLD_LAZY);
	if (!handle) {
		std::cerr << dlerror() << std::endl;
		return 1;
	}
	auto bruh = dlsym(handle, "_Z18GetDoubleFunctionsv");
	std::cerr << (bruh) << " "  << std::endl;
	auto double_func = reinterpret_cast<dfunc>(bruh);
	// std::cerr << (double_func) << " " << (*double_func).target_type().name() << std::endl;
	auto double_funcs = (*double_func)();
	// dlclose(handle);
	// auto double_funcs = GetDoubleFunctions();
	std::cerr << (double_funcs).size() << std::endl;
	for (int i = 0; i < 20; ++i) {
		for (auto func : double_funcs) {
			std::cout << func() << std::endl;
		}
		std::cout << "------------" << std::endl;
		sleep(5);
	}
	dlclose(handle);
}