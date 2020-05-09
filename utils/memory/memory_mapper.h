
#pragma once
#include <sys/mman.h>
#include <stdint.h>

namespace mm {
    class mem_mapper {
        static mem_mapper* instance_;

        mem_mapper();
        void init();

    public:
        uint32_t granularity();

        const char* curr_dir();
        bool dir_exists(const char* dir);
        bool file_exists(const char* filepath);
        bool create_dir(const char* dir);
        void delete_dir_and_contents(const char* dir);

        char* map(const char* filepath, uint32_t sz);
        void unmap(char* ref, uint32_t sz);

        static mem_mapper& instance();
    };

#define MM_INSTANCE mm::mem_mapper::instance()
}