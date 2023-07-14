#include "cpu.h"

double GetCPULoad() {
	static size_t	working = 0;
	static size_t	total = 0;
	size_t			new_working = 0;
	size_t			new_total = 0;
	size_t			temp;
	double			load = 0;
	std::fstream	stat = std::fstream("/proc/stat", std::ios_base::in);
	std::string		name;

	stat >> name;
	if (name == "cpu") {
		for (int i = 0; i < 3; ++i) {
			stat >> temp;
			new_working += temp;
		}
		new_total = new_working;
		for (int i = 0; i < 7; ++i) {
			stat >> temp;
			new_total += temp;
		}
		load = (100 * (double)(new_working - working)) / (new_total - total);
		working = new_working;
		total = new_total;
	}
	return load;
}

// Returns count of processes which we can get with ps aux.
// I'm not sure if using ps aux is the correct way to get all working processes.
int GetNumberOfProcesses() {
	int nop;
	redi::ipstream in("ps aux | wc -l");
	in >> nop;
	return nop - 1;
}

std::vector<std::function<int ()>>		GetIntegerFunctions() {
	return {GetNumberOfProcesses};
}

std::vector<std::function<double ()>>	GetDoubleFunctions() {
	return {GetCPULoad};
}

extern "C" {
	double cpu() {
		return GetCPULoad();
	}

	int processes() {
		return GetNumberOfProcesses();
	}
}