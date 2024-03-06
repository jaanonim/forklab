cmake_minimum_required(VERSION 3.27)
project(forklab)

set(CMAKE_CXX_STANDARD 23)
include_directories(include lib src)


file(GLOB all_SRCS
        "src/*.cpp"
        "src/*.c"
)

add_executable(forklab main.cpp ${all_SRCS})
