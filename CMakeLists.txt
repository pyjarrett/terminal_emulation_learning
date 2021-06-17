cmake_minimum_required(VERSION 3.17)
project(vt100_compat)

set(CMAKE_CXX_STANDARD 17)

set(VT100_COMMON_CXX_FLAGS "${VT100_COMMON_CXX_FLAGS} -Wall -Wextra")

set(VT100_SRCS
    src/errors.cpp
    )

if (WIN32)
    set(VT100_SRCS
        ${VT100_SRCS}
        src/windows_modes.cpp
        )
endif(WIN32)

if (WIN32)
    add_executable(print_windows_modes src/print_windows_modes.cpp ${VT100_SRCS})
endif(WIN32)
