cmake_minimum_required(VERSION 3.10)
project (memory_map_tests)

set (CMAKE_CXX_STANDARD 17)

set (SRCS main.cpp utils/memory/memory_mapper.cpp)
add_executable (memory_map_tests ${SRCS})

target_link_libraries (memory_map_tests cap stdc++fs)
