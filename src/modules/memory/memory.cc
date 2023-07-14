#include "memory.h"

double GetTotalRAM() {
    long pages = sysconf(_SC_PHYS_PAGES);
    long page_size = sysconf(_SC_PAGE_SIZE);
    return pages * page_size;
}

double GetFreeRAM() {
    long pages = sysconf(_SC_AVPHYS_PAGES);
    long page_size = sysconf(_SC_PAGE_SIZE);
    return pages * page_size;
}

double GetUsedDisk() {
    const std::filesystem::space_info si = std::filesystem::space("/");
    return (1 - (double)si.free / si.capacity) * 100;
}

extern "C" {
    double ram_total () {
        return GetTotalRAM();
    }

    double ram () {
        return ((GetTotalRAM() - GetFreeRAM()) / GetTotalRAM()) * 100;
    }

    double hard_volume () {
        return GetUsedDisk();
    }
}