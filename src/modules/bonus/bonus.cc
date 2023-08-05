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

    double cpu_idle_usage() {
        s21::cpu::update_values();
        return s21::cpu::cpu_idle_usage();
    }         // 4

    double cpu_user_usage(){
        return s21::cpu::cpu_user_usage();    
    }         // 1

    double cpu_privileged_usage(){
        return s21::cpu::cpu_privileged_usage();    
    }   // 3
    
    double cpu_dpc_usage(){
        return s21::cpu::cpu_dpc_usage();
    }          // ??
    
    double cpu_interrupt_usage(){
        return s21::cpu::cpu_interrupt_usage();
    }    // 6

    double total_swap() {
        return s21::memory::total_swap();
    }

    double free_swap() {
        return s21::memory::free_swap();
    }

    double virtual_mem_volume(){
        return s21::memory::virtual_mem_volume();
    }

    double virtual_mem_free(){
        return s21::memory::virtual_mem_free();
    }

    int proc_queue_length() {
        int             answer;
        redi::ipstream  command("ps -e --format=\"stat\" | grep \"R\" | wc -l");
        command >> answer;
        return answer - 1; // Subtracting 1 because ps call also counts as runnable
    }

    int inodes(){ // df -i /
        int             all_inodes, used_inodes;
        std::string     line, partition;
        redi::ipstream  command("df -i /");
        
        std::getline(command, line);
        command >> partition >> all_inodes >> used_inodes;
        return used_inodes;
    }

    double hard_read_time(){
        return -1;
    }

    int system_errors() { // journalctl -p 3 -b | wc -l
        int             error_count;
        redi::ipstream  command("journalctl -p 3 -b | wc -l");
        command >> error_count;
        return error_count;
    }

    // Not entirely correct since it counts every opened tty and pts
    int user_auths(){ // last -p now | ws -l 
        int             user_auths;
        redi::ipstream  command("last -p now | ws -l");
        command >> user_auths;
        return user_auths - 3;
    }

}