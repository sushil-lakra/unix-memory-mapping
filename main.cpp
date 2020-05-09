
#include "utils/unix_utils/cmd_utils.h"
#include "utils/memory/memory_mapper.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>

#include <unistd.h>
#include <errno.h>
#include <fstream>

#include <iomanip> //setprecision

#define _1k 1024

char* shmget_addr();
void set_required_capabilities();
void process_mem_usage();
void memory_mapper_tests();

int main() {
    memory_mapper_tests();
}

void memory_mapper_tests() {
    auto cwd = MM_INSTANCE.curr_dir();
    std::cout << cwd << std::endl;
    delete cwd;

    auto new_dir = "temp";
    MM_INSTANCE.create_dir(new_dir);

    std::cout << "dir_exists : " << MM_INSTANCE.dir_exists(new_dir) << std::endl;
    std::cout << "this_directory_does_not_exist : " << MM_INSTANCE.dir_exists("this_directory_does_not_exist") << std::endl;
    std::ofstream ofs("file");
    std::cout << "ofs.is_open() : " << ofs.is_open() << std::endl;
    std::cout << "file should not be taken as directory: " << MM_INSTANCE.dir_exists("file") << std::endl;

    auto new_dir_1 = "temp1";
    MM_INSTANCE.create_dir(new_dir_1);
    MM_INSTANCE.delete_dir_and_contents(new_dir_1);

    auto map_file = "map_file";
    auto buf = MM_INSTANCE.map(map_file, MM_INSTANCE.granularity() * 10);
    for (int i = 0; i < 1000; ++i) {
        buf[i] = '1';
    }
    
    std::cout << "buf : " << size_t(buf) << std::endl;
    MM_INSTANCE.unmap(buf, MM_INSTANCE.granularity() * 10);
}

char* shmget_addr() {
    int page_size = getpagesize();
    std::cout << "page_size : " << page_size << std::endl;
    int mem_size = page_size * _1k;

    // ftok to generate unique key 
    key_t key = ftok("/mnt/d/wsl/tests/cpptests/shmfile", 1); 
    //key produced for referred filename and proj id is always same.

    int shmid = shmget(key, mem_size, IPC_CREAT | 0777);

    if (shmid == -1) {
        perror("shared memory initialization failed");
    }

    std::cout << "shmid : " << shmid << std::endl;

    // shmat to attach to shared memory 
    char *ptr = (char*) shmat(shmid,(void*)0,0); 

    //write to address
    for (int i = 0; i < mem_size; ++i) {
        ptr[i] = 'a';
    }
    
    //read and verify
    for (int i = 0; i < mem_size; ++i) {
        if (ptr[i] != 'a') {
            std::cout << "data mismatch" << std::endl;
        }
    }

    std::cout << "ptr : " << (size_t)ptr << std::endl;
    return ptr;
}

/*
not found - sudo apt-get install libcap-dev
note: capabilities are user specific, most prbably running this with normal user wont be sufficient,
because user can't give additional privileges to himself.
*/
/*
#include <sys/capability.h>
void set_required_capabilities() {
    cap_t caps;
    
    caps = cap_get_proc();
    if (caps == NULL)
        perror("Unable to get capabilities");
    
    size_t nu_caps = 3;
    cap_value_t cap_list[nu_caps];
    cap_list[0] = CAP_FOWNER;
    cap_list[1] = CAP_SETFCAP;
    cap_list[2] = CAP_SYS_RESOURCE;
    if (cap_set_flag(caps, CAP_EFFECTIVE, nu_caps, cap_list, CAP_SET) == -1)
        perror("Unable to get capabilities");

    ssize_t size = 0;
    char* caps_str = cap_to_text(caps, &size);
    std::cout << "caps_str : \"" << caps_str
        << "\" size = " << size << std::endl;

}
*/

void process_mem_usage()
{
    int page_size = getpagesize();
    std::cout << "page_size : " << page_size << std::endl;
    auto size = 1024 * page_size;
    char* buffer = new char[size];
    std::string cmd = "top";
    auto pid = getpid();
    cmd += " -p "  + std::to_string(pid) + " -n 1";

    auto stats = execute_top(cmd.c_str());
    std::cout << "stats.virt : " << stats.virt << " stats.res : " << stats.res << std::endl;

    for (auto i = 0; i < size; ++i)
        buffer[i] = 'a';

    stats = execute_top(cmd.c_str());
    std::cout << "stats.virt : " << stats.virt << " stats.res : " << stats.res << std::endl;

    delete[] buffer;
    stats = execute_top(cmd.c_str());
    std::cout << "stats.virt : " << stats.virt << " stats.res : " << stats.res << std::endl;
}
