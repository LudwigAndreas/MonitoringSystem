#include "module_header.h"

#include <iostream>
#include <functional>
#include <unistd.h>
#include <vector>
#include <dlfcn.h>
#include <cstdlib>

typedef std::vector<std::function<double ()>> dvec;
typedef std::vector<std::function<int ()>>	  ivec;

typedef double (*dfunc)();
typedef int (*ifunc)();

struct Library {
	std::string name;
	void		*handle;
	dvec		dfuncs;
	ivec		ifuncs;
};


// Library OpenLibrary

int main() {
	void *cpuhandle = dlopen("/home/andref/Documents/s21/MonitoringSystem/MonitoringSystem/src/modules/libmscpu.so", RTLD_LAZY);
	void *memhandle = dlopen("/home/andref/Documents/s21/MonitoringSystem/MonitoringSystem/src/modules/libmsmem.so", RTLD_LAZY);
	if (!cpuhandle || !memhandle) {
		std::cerr << dlerror() << std::endl;
		return 1;
	}
	auto cpu_addr = dlsym(cpuhandle, "cpu");
	auto processes_addr = dlsym(cpuhandle, "processes");
	auto ram_total_addr = dlsym(memhandle, "ram_total");
	auto ram_addr = dlsym(memhandle, "ram");
	auto hard_volume_addr = dlsym(memhandle, "hard_volume");
	// std::cerr << (cpu_addr) << " " << processes_addr << std::endl;
	auto cpu = reinterpret_cast<dfunc>(cpu_addr);
	auto processes = reinterpret_cast<ifunc>(processes_addr);
	auto ram_total = reinterpret_cast<dfunc>(ram_total_addr);
	auto ram = reinterpret_cast<dfunc>(ram_addr);
	auto hard_volume = reinterpret_cast<dfunc>(hard_volume_addr);
	for (int i = 0; i < 5; ++i) {
		// for (auto func : double_funcs) {
		std::cout << "CPU Load: " << cpu() << "%" << std::endl
				  << "Processes: " << processes() << std::endl
				  << "RAM Total: " << ram_total() / 1073741824 << "GB" << std::endl
				  << "RAM Usage: " << ram() << "%" << std::endl
				  << "Disk Usage: " << hard_volume() << "%" << std::endl;
		// }
		std::cout << "------------" << std::endl;
		sleep(5);
	}
	dlclose(cpuhandle);
	dlclose(memhandle);
}