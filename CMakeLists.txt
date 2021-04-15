cmake_minimum_required(VERSION 3.17)
project(mhserv C)

if (HAIKU)
    add_compile_definitions(HAIKU)
    add_compile_definitions(_BSD_SOURCE)
endif(HAIKU)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(MH_DEBUG)
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS_DEBUG "-ggdb3")
set(CMAKE_C_FLAGS_RELEASE "-O3")

# libmh.so
add_library(mh SHARED mh/streams/mh_stream.c mh/streams/mh_memory_stream.c mh/mh_memory.c mh/network/mh_tcp.c mh/streams/mh_socket_stream.c mh/network/mh_http.c mh/mh_thread.c mh/streams/mh_file_stream.c mh/mh_context.c mh/collections/mh_shitmap.c mh/collections/mh_collection.c mh/mh_handle.c)

if (WIN32)
    target_link_libraries(mh PRIVATE wsock32 ws2_32 dl)
endif (WIN32)

if(HAIKU)
    target_link_libraries(mh PRIVATE pthread bsd network)
endif(HAIKU)

if(UNIX AND NOT (APPLE OR HAIKU))
    target_link_libraries(mh PRIVATE pthread dl)
endif(UNIX AND NOT (APPLE OR HAIKU))

# mhserv
add_executable(mhserv mhserv.c)
target_link_libraries(mhserv PRIVATE mh)

# libexample.so
add_library(example SHARED example.c)
target_link_libraries(example PUBLIC mh)