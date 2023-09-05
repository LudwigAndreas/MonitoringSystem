#include "network.h"

int streams = 0;

int GetURLAvailability(std::string url) {
  int exit_status;
  std::stringstream ss;
  ss << std::fixed << "ping " << url << " -W 0.3 -qc 1 >/dev/null 2>&1; echo $?";
  redi::ipstream in(ss.str());
  in >> exit_status;
  return (exit_status == 0);
}

#if __linux__
uint64_t ReadThroughputLine(std::istream &is) {
  std::string interface_name;
  uint64_t ibytes, ipackets, ierr, idrop, ififo, iframe, icompressed, imulticast,
      obytes, opackets, oerr, odrop, ofifo, oframe, ocompressed, omulticast;
  is >> interface_name >> ibytes >> ipackets >> ierr >> idrop >> ififo >> iframe >> icompressed >> imulticast >>
              obytes >> opackets >> oerr >> odrop >> ofifo >> oframe >> ocompressed >> omulticast;
//    LOG_DEBUG(interface_name << " IN - " << ibytes << "B : OUT - " << obytes << "B");
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
}

#elif __APPLE__

bool getNetworkTraffic(const char* interfaceName, unsigned long long& bytesIn, unsigned long long& bytesOut) {
  struct ifaddrs* ifaddr;
  if (getifaddrs(&ifaddr) == -1) {
    std::cerr << "Error getting network interface information." << std::endl;
    return false;
  }

  for (struct ifaddrs* ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
    if (ifa->ifa_addr == nullptr || ifa->ifa_data == nullptr)
      continue;

    if (ifa->ifa_addr->sa_family == AF_LINK && strcmp(ifa->ifa_name, interfaceName) == 0) {
      struct if_data* if_data = static_cast<struct if_data*>(ifa->ifa_data);
      bytesIn = if_data->ifi_ibytes;
      bytesOut = if_data->ifi_obytes;
      freeifaddrs(ifaddr);
      return true;
    }
  }

  freeifaddrs(ifaddr);
  return false;
}

double GetNetworkThroughput() {
  static uint64_t io = 0;
  uint64_t old_io = io;
  struct ifaddrs* ifaddr;
  if (getifaddrs(&ifaddr) == -1) {
    std::cerr << "Error getting network interface information." << std::endl;
    return 1;
  }

  for (struct ifaddrs* ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
    if (ifa->ifa_addr == nullptr || ifa->ifa_data == nullptr)
      continue;

    if (ifa->ifa_addr->sa_family == AF_LINK) {
      unsigned long long bytesIn = 0;
      unsigned long long bytesOut = 0;

      if (getNetworkTraffic(ifa->ifa_name, bytesIn, bytesOut)) {
        // std::cout << "Interface: " << ifa->ifa_name << std::endl;
        // std::cout << "Traffic Bytes In: " << bytesIn << " bytes" << std::endl;
        // std::cout << "Traffic Bytes Out: " << bytesOut << " bytes" << std::endl;
        io += bytesIn;
        io += bytesOut;
      } else {
        std::cerr << "Unable to get network traffic data for interface " << ifa->ifa_name << std::endl;
      }
    }
  }

  freeifaddrs(ifaddr);
  if (old_io)
    return (double)(io - old_io);
  else
    return 0;
}

#endif

std::string url(std::string url) {
  std::stringstream ss;
  ss << std::fixed << GetURLAvailability(url);
  return ss.str();
}

std::string inet_throughput() {
  std::stringstream ss;
  ss << std::fixed << GetNetworkThroughput();
  return ss.str();
}