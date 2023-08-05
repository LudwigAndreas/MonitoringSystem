#include "network.h"

int streams = 0;

int GetURLAvailability(std::string url) {
	int exit_status;
    std::stringstream ss;
    ss << "ping " << url << " -W 0.3 -qc 1 >/dev/null 2>&1; echo $?";
	redi::ipstream in(ss.str());
    in >> exit_status;
    return (exit_status == 0);
}

uint64_t ReadThroughputLine(std::istream &is) {
    std::string interface_name;
    uint64_t ibytes, ipackets, ierr, idrop, ififo, iframe, icompressed, imulticast,
             obytes, opackets, oerr, odrop, ofifo, oframe, ocompressed, omulticast;

    is >> interface_name >> ibytes >> ipackets >> ierr >> idrop >> ififo >> iframe >> icompressed >> imulticast >>
                              obytes >> opackets >> oerr >> odrop >> ofifo >> oframe >> ocompressed >> omulticast;
    LOG_DEBUG(interface_name << " IN - " << ibytes << "B : OUT - " << obytes << "B");
    if (!is)
        return (std::numeric_limits<uint64_t>::max());
    return ibytes + obytes;
}

double GetNetworkThroughput() {
    std::ifstream file("/proc/net/dev");
    static uint64_t io = 0;
    uint64_t old_io = io;
    // uint64_t new_io = 0;
    uint64_t protocol_io;
    uint64_t ibytes, ipackets, ierr, idrop, ififo, iframe, icompressed, imulticast,
             obytes, opackets, oerr, odrop, ofifo, oframe, ocompressed, omulticast;
    std::string tmp;

    io = 0;
    std::getline(file, tmp); // Header line
    std::getline(file, tmp); // Header line
    while (true) {
        protocol_io = ReadThroughputLine(file);
        if (protocol_io == std::numeric_limits<uint64_t>::max())
            break;
        io += protocol_io;
    }
    file.close();
    // io += reads_completed + writes_completed;
    if (old_io)
        return (double)(io - old_io);
    else
        return 0;
    // return (1 - (double)si.free / si.capacity) * 100;
}

extern "C" {
    int url(std::string url) {
        if (!streams) {
            logger.AddOutputStream(std::make_shared<std::fstream("log.log", std::fstream::trunc | std::fstream::in | std::fstream::out), true, s21::diagnostic::LogLevel::Trace);
            ++streams;
        }
        return GetURLAvailability(url);
    }

    double inet_throughput() {
        if (!streams) {
            logger.AddOutputStream(std::make_shared<std::fstream("log.log", std::fstream::trunc | std::fstream::out), true, s21::diagnostic::LogLevel::Trace);
            ++streams;
        }
        return GetNetworkThroughput();
    }
}