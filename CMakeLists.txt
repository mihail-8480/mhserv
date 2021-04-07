cmake_minimum_required(VERSION 3.17)
project(mhserv C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror -fsanitize=address,undefined")
set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -fwhole-program")
set(CMAKE_EXE_LINKER_FLAGS "-static -pthread")

add_executable(mhserv main.c mh/mh_stream.c mh/mh_memory_stream.c mh/mh_memory.c mh/mh_tcp.c mh/mh_socket_stream.c mh/mh_http.c mh/mh_console.c mh/mh_thread.c mh/mh_file_stream.c mh/mh_context.c)