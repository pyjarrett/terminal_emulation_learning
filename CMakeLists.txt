cmake_minimum_required(VERSION 3.17)
project(terminal_emulation_learning)

set(CMAKE_CXX_STANDARD 17)

set(TEL_COMMON_CXX_FLAGS "${TEL_COMMON_CXX_FLAGS} -Wall -Wextra")

set(TEL_SRCS
    src/errors.cpp
    src/terminal.h
    )

if (WIN32)
    set(TEL_SRCS
        ${TEL_SRCS}
        src/windows/windows_modes.cpp
        )

    add_executable(print_windows_modes
        src/examples/print_windows_modes.cpp
        ${TEL_SRCS})
endif(WIN32)

if (UNIX)
    set(TEL_SRCS
        ${TEL_SRCS}
        src/linux/linux_terminal.cpp
        )

    add_executable(linux_example
        src/examples/linux_example.cpp
        ${TEL_SRCS})
endif (UNIX)
