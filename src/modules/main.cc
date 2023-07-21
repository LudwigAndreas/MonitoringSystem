#include "module_header.h"
#include "../../libs21/test/utils/logger.inc"

#include <iostream>
#include <functional>
#include <unistd.h>
#include <vector>
#include <dlfcn.h>
#include <cstdlib>
#include <fstream>

typedef std::vector<std::function<double ()>> dvec;
typedef std::vector<std::function<int ()>>	  ivec;

typedef double (*dfunc)();
typedef int (*ifunc)();
typedef int (*isfunc)(std::string);

struct Library {
	std::string name;
	void		*handle;
	dvec		dfuncs;
	ivec		ifuncs;
};


// Library OpenLibrary

int main() {
	int sleep_time = 5;

	void *cpuhandle = dlopen("/home/andref/Documents/s21/MonitoringSystem/MonitoringSystem/src/modules/libmscpu.so", RTLD_LAZY);
	void *memhandle = dlopen("/home/andref/Documents/s21/MonitoringSystem/MonitoringSystem/src/modules/libmsmem.so", RTLD_LAZY);
	void *nethandle = dlopen("/home/andref/Documents/s21/MonitoringSystem/MonitoringSystem/src/modules/libmsnet.so", RTLD_LAZY);
	if (!cpuhandle || !memhandle || !nethandle) {
		std::cerr << dlerror() << std::endl;
		return 1;
	}
	auto cpu_addr = dlsym(cpuhandle, "cpu");
	auto processes_addr = dlsym(cpuhandle, "processes");
	auto ram_total_addr = dlsym(memhandle, "ram_total");
	auto ram_addr = dlsym(memhandle, "ram");
	auto hard_volume_addr = dlsym(memhandle, "hard_volume");
	auto hard_ops_addr = dlsym(memhandle, "hard_ops");
	auto url_addr = dlsym(nethandle, "url");
	auto inet_throughput_addr = dlsym(nethandle, "inet_throughput");
	// std::cerr << (cpu_addr) << " " << processes_addr << std::endl;
	auto cpu = reinterpret_cast<dfunc>(cpu_addr);
	auto processes = reinterpret_cast<ifunc>(processes_addr);
	auto ram_total = reinterpret_cast<dfunc>(ram_total_addr);
	auto ram = reinterpret_cast<dfunc>(ram_addr);
	auto hard_volume = reinterpret_cast<dfunc>(hard_volume_addr);
	auto hard_ops = reinterpret_cast<ifunc>(hard_ops_addr);
	auto url = reinterpret_cast<isfunc>(url_addr);
	auto inet_throughput = reinterpret_cast<dfunc>(inet_throughput_addr);
	for (int i = 0; i < 5; ++i) {
		// for (auto func : double_funcs) {
		std::cout << "CPU Load: " << cpu() << "%" << std::endl
				  << "Processes: " << processes() << std::endl

				  << "RAM Total: " << ram_total() / 1073741824 << "GiB" << std::endl
				  << "RAM Usage: " << ram() << "%" << std::endl
				  << "Disk Usage: " << hard_volume() << "%" << std::endl
				  << "Disk IO operations: " << (double)hard_ops() / sleep_time << "ops/s" << std::endl
				  
				  << "8.8.8.8 Availability: " << std::boolalpha << (bool)url("8.8.8.8") << std::endl
				  << "Net Throughput: " << inet_throughput() / (1000000 * sleep_time) << "KB/s" << std::endl;
		// }
		std::cout << "------------" << std::endl;
		sleep(sleep_time);
	}
	dlclose(cpuhandle);
	dlclose(memhandle);
	dlclose(nethandle);
}