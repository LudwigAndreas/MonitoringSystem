#include "bonus.h"
#include <numeric>

namespace s21 {

    class cpu {  
        enum cpu_type {
            user,
            nice,
            system,
            idle,
            iowait,
            irq,
            softirq,
            steal,
            guest,
            guest_nice,
            SIZE
        };
        static std::vector<double> cpu_usage;
        static std::vector<double> last_usage;
        static std::vector<double> current_usage;

        public:
        static void update_values() {
        	std::fstream	stat = std::fstream("/proc/stat", std::ios_base::in);
    	    std::string		name;
            double          value;

        	stat >> name;
            if (name == "cpu") {
                if (current_usage.empty())
                    last_usage = std::vector<double>(cpu_type::SIZE, 0.);
                else
                    last_usage = std::move(current_usage);
                current_usage = {};
                for (int i = 0; i < cpu_type::SIZE; ++i) {
                    stat >> value;
                    current_usage.push_back(value);
                }
                std::transform(current_usage.begin(), current_usage.end(), last_usage.begin(), cpu_usage.begin(), std::minus<double>());
            }
            double sum = std::accumulate(cpu_usage.begin(), cpu_usage.end(), 0.) * .01;
            std::transform(cpu_usage.begin(), cpu_usage.end(), cpu_usage.begin(), [sum](double value){return value / sum;});
        }

        static double cpu_idle_usage(){
            return cpu_usage[cpu_type::idle];
        }         // 4

        static double cpu_user_usage(){
            return cpu_usage[cpu_type::user];    
        }         // 1

        static double cpu_privileged_usage(){
            return cpu_usage[cpu_type::system];
        }   // 3

        static double cpu_dpc_usage(){
            return cpu_usage[cpu_type::nice];
        }          // ??

        static double cpu_interrupt_usage(){
            return cpu_usage[cpu_type::irq];    
        }    // 6

    };

    class memory {
        static std::map<std::string, std::string> meminfo;
        public:
        static void update_values() {
        	meminfo = s21::properties_reader("/proc/stat", ':');
    	}

        static double get_value(std::string key) {
            double              numeric;
            std::istringstream  ss(meminfo.at(key));
            ss >> numeric;
            return numeric;
        }

        static double total_ram() {
            return get_value("MemTotal");
        }

        static double free_ram() {
            return get_value("MemAvailable");
        }

        static double total_swap() {
            return get_value("SwapTotal");
        }

        static double free_swap() {
            return get_value("SwapFree");
        }

        static double virtual_mem_volume(){
            return memory::total_swap() + memory::total_ram();
        }

        static double virtual_mem_free(){
            return memory::free_swap() + memory::free_ram();
        }
    };
}

        // static s21::cpu cpu;

extern "C" {

    std::string cpu_idle_usage() {
        s21::cpu::update_values();
        std::stringstream ss;
        ss << s21::cpu::cpu_idle_usage();
        return ss.str();
    }         // 4

    std::string cpu_user_usage(){
        std::stringstream ss;
        ss << s21::cpu::cpu_user_usage();
        return ss.str();
    }         // 1

    std::string cpu_privileged_usage(){
        std::stringstream ss;
        ss << s21::cpu::cpu_privileged_usage();
        return ss.str();
    }   // 3
    
    std::string cpu_dpc_usage(){
        std::stringstream ss;
        ss << s21::cpu::cpu_dpc_usage();
        return ss.str();
    }          // ??
    
    std::string cpu_interrupt_usage(){
        std::stringstream ss;
        ss << s21::cpu::cpu_interrupt_usage();
        return ss.str();
    }    // 6

    std::string total_swap() {
        std::stringstream ss;
        ss << s21::memory::total_swap();
        return ss.str();
    }

    std::string free_swap() {
        std::stringstream ss;
        ss << s21::memory::free_swap();
        return ss.str();
    }

    std::string virtual_mem_volume(){
        std::stringstream ss;
        ss << s21::memory::virtual_mem_volume();
        return ss.str();
    }

    std::string virtual_mem_free(){
        std::stringstream ss;
        ss << s21::memory::virtual_mem_free();
        return ss.str();
    }

    std::string proc_queue_length() {
        int             answer;
        redi::ipstream  command("ps -e --format=\"stat\" | grep \"R\" | wc -l");
        std::stringstream ss;

        command >> answer;
        ss << answer - 1; // Subtracting 1 because ps call also counts as runnable
        return ss.str(); 
    }

    std::string inodes(){ // df -i /
        int             all_inodes, used_inodes;
        std::string     line, partition;
        std::stringstream ss;
        redi::ipstream  command("df -i /");
        
        std::getline(command, line);
        command >> partition >> all_inodes >> used_inodes;
        ss << used_inodes;
        return ss.str();
    }

    std::string hard_read_time(){
        return "Sudondese";
    }

    std::string system_errors() { // journalctl -p 3 -b | wc -l
        int             error_count;
        std::stringstream ss;
        redi::ipstream  command("journalctl -p 3 -b | wc -l");
        
        command >> error_count;
        ss << error_count;
        return ss.str();
    }

    // Not entirely correct since it counts every opened tty and pts
    std::string user_auths(){ // last -p now | ws -l 
        int             user_auths;
        std::stringstream ss;
        redi::ipstream  command("last -p now | ws -l");

        command >> user_auths;
        ss << user_auths - 3;
        return ss.str();
    }

}