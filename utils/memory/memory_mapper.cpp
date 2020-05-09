
#include "memory_mapper.h"
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include <fcntl.h>
#include <errno.h>

//#include <filesystem>
#include <experimental/filesystem>

namespace mm {
    mem_mapper* mem_mapper::instance_ = nullptr;
    namespace {
        uint32_t page_size_;
    }

    mem_mapper::mem_mapper() {}

    void
    mem_mapper::init() {
        page_size_ = getpagesize();
    }

    uint32_t
    mem_mapper::granularity() {
        return page_size_;
    }

    const char*
    mem_mapper::curr_dir() {
        static const size_t PATH_MAX = 307;
        char* cwd = new char[PATH_MAX];
        strcpy(cwd, std::experimental::filesystem::current_path().c_str());
        return cwd;
    }

    bool
    mem_mapper::dir_exists(const char* dir) {
        return std::experimental::filesystem::exists(dir) &&
            std::experimental::filesystem::is_directory(dir);
    }

    bool
    mem_mapper::file_exists(const char* filepath) {
        std::ifstream ifs(filepath);
        return ifs.is_open();
    }

    bool
    mem_mapper::create_dir(const char* dir) {
        return std::experimental::filesystem::create_directory(dir);
    }

    void
    mem_mapper::delete_dir_and_contents(const char* dir) {
        std::experimental::filesystem::remove_all(dir);
    }

    mem_mapper&
    mem_mapper::instance()
    {
        if (instance_ == nullptr)
        {
            instance_ = new mem_mapper;
            instance_->init();
        }
        return *instance_;
    }

    char*
    mem_mapper::map(const char* filepath, uint32_t sz) {
        int mode = 0x0777;
        auto fd = open(filepath, O_RDWR | O_CREAT, mode);

        if (fd < 0) {
            perror("Failed to open file");
            return nullptr;
        }

        sz = sz / page_size_ * page_size_;

        std::cout << sz <<std::endl;
        char* buf = static_cast<char*>(
            mmap(0,
                sz,
                PROT_READ | PROT_WRITE,
                MAP_PRIVATE,
                fd,
                0
            )
        );
        if (buf == MAP_FAILED) {
            perror("Failed to map file");
            return nullptr;
        }
        return buf;
    }
    void
    mem_mapper::unmap(char* ref, uint32_t sz) {

    }
}