cmake_minimum_required(VERSION 3.17)
project(vt100_compat)

set(CMAKE_CXX_STANDARD 17)

set(VT100_COMMON_CXX_FLAGS "${VT100_COMMON_CXX_FLAGS} -Wall -Wextra")

add_executable(vt100_compat main.cpp)
add_executable(print_all_modes print_all_modes.cpp)