
#include "../str_utils/string_stripper.h"

#include  <iostream>
#include <memory>
#include <algorithm>
#include <cstring>

std::string execute_command_and_return_output(const char* cmd) {
    std::string res;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        std::cout << "error executing commande : " << cmd << std::endl;
    } else {
        std::array<char, 128> buffer;
        std::string tmp;
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            res += buffer.data();
        }

        strip_spaces(res);
        return res;
    }

    return "";
}

#include <sstream>
struct top_info {
    uint32_t    pid;
    std::string user;
    uint32_t    pr;         //priority
    uint32_t    ni;         //nice values of a process
    uint32_t    virt;       //memory consumed by process, i.e. total memory
    uint32_t    res;        //resident set, memory in ram
    uint32_t    shr;        //memory shared
    std::string s;          //state of process
    double      per_mem;    //percent of memory
    double      per_cpu;    //percent of cpu
    std::string time;       //time process running
    std::string cmd;        //command executing

    top_info(const std::string& str) {
        std::stringstream ss(str);
        ss >> pid >> user >> pr >> ni >> virt >> res >> shr >> s >> per_mem >> per_cpu >> time >> cmd;
    }

    top_info() :
        pid{0},
        user(""),
        pr{0},
        ni{0},
        virt{0},
        res{0},
        shr{0},
        s(""),
        per_mem{0.0},
        per_cpu{0.0},
        time(""),
        cmd("")
    {}

    void print() {
        std::cout << "pid:" << pid << " user:" << user << " pr:" << pr << " ni:" << ni << " virt:" << virt
            << " res:" << res << " shr:" << shr << " s:" << s << " per_mem:" << per_mem << " per_cpu:" << per_cpu
            << " time:" << time << " cmd:" << cmd << std::endl;
    }
};

top_info execute_top(const char* cmd) {
    std::string res;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        std::cout << "error executing commande : " << cmd << std::endl;
    } else {
        std::array<char, 128> buffer;
        std::string tmp;
        bool read_next_line = false;
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            res = buffer.data();
            if (!read_next_line) {
                if (res.find("PID USER") != std::string::npos) {
                    //std::cout << "res: " << res << std::endl;
                    read_next_line = true;
                }
            } else {
                break;
            }
        }
        fgets(buffer.data(), buffer.size(), pipe.get());
        fgets(buffer.data(), buffer.size(), pipe.get());
        res = buffer.data();
        res = res.substr(7, res.length() - 7);
        return top_info(res);
    }
    return top_info();
}

void test_execute_top() {
    auto v = execute_top("top -n 1");
    v.print();
}