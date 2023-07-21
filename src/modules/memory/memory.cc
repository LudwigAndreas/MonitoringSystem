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

int GetDiskIO() {
    std::ifstream file("/proc/diskstats");
    static int io = 0;
    int old_io = io;
    unsigned int disk_id, partition_id, reads_completed, reads_merged, sectors_read, ms_spent_reading, writes_completed, writes_merged, sectors_written, ms_spent_writing, ios_in_progress, ms_spent_on_ios, weighted_ms_spent_on_ios;
    std::string partition_name;

    file >> disk_id >> partition_id >> partition_name >> reads_completed >> reads_merged >> sectors_read >> ms_spent_reading >> writes_completed >> writes_merged >> sectors_written >> ms_spent_writing >> ios_in_progress >> ms_spent_on_ios >> weighted_ms_spent_on_ios;
    file.close();
    io = reads_completed + writes_completed;
    if (old_io)
        return io - old_io;
    else
        return 0;
    // return (1 - (double)si.free / si.capacity) * 100;
}

extern "C" {
    double ram_total() {
        return GetTotalRAM();
    }

    double ram() {
        return ((GetTotalRAM() - GetFreeRAM()) / GetTotalRAM()) * 100;
    }

    double hard_volume() {
        return GetUsedDisk();
    }

    int hard_ops() {
        return GetDiskIO();
    }
}