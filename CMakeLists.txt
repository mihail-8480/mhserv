cmake_minimum_required(VERSION 3.17)
project(mhserv C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror -fsanitize=address,undefined")
set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -fwhole-program")
set(CMAKE_EXE_LINKER_FLAGS "-pthread -ldl")

add_library(mh SHARED mh/streams/mh_stream.c mh/streams/mh_memory_stream.c mh/mh_memory.c mh/network/mh_tcp.c mh/streams/mh_socket_stream.c mh/network/mh_http.c mh/mh_thread.c mh/streams/mh_file_stream.c mh/mh_context.c mh/collections/mh_shitmap.c mh/collections/mh_collection.c)
add_executable(mhserv main.c)
target_link_libraries(mhserv PUBLIC mh)
add_library(example SHARED example.c)
target_link_libraries(example PUBLIC mh)
