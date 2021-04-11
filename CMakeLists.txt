cmake_minimum_required(VERSION 3.17)
project(mhserv C)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# To enable mh_context debug output, uncomment the next line:
#add_compile_definitions(MH_DEBUG)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS_DEBUG "-ggdb3")
set(CMAKE_C_FLAGS_RELEASE "-O3")

# libmh.so
add_library(mh SHARED mh/streams/mh_stream.c mh/streams/mh_memory_stream.c mh/mh_memory.c mh/network/mh_tcp.c mh/streams/mh_socket_stream.c mh/network/mh_http.c mh/mh_thread.c mh/streams/mh_file_stream.c mh/mh_context.c mh/collections/mh_shitmap.c mh/collections/mh_collection.c mh/mh_handle.c)
target_link_libraries(mh PRIVATE Threads::Threads dl)

# mhserv
add_executable(mhserv main.c)
target_link_libraries(mhserv PUBLIC mh)

# libexample.so
add_library(example SHARED example.c)
target_link_libraries(example PUBLIC mh)