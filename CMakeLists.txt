cmake_minimum_required(VERSION 3.17)
project(mhserv C)

if (HAIKU)
    add_compile_definitions(HAIKU)
endif(HAIKU)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(MH_DEBUG)
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")

if (NOT HAIKU)
    set(CMAKE_C_STANDARD 99)
endif(NOT HAIKU)
set(CMAKE_C_FLAGS_DEBUG "-ggdb3")
set(CMAKE_C_FLAGS_RELEASE "-O3")

# libmh.so
add_library(mh SHARED mh/streams/mh_stream.c mh/streams/mh_memory_stream.c mh/mh_memory.c mh/network/mh_tcp.c mh/streams/mh_socket_stream.c mh/network/mh_http.c mh/mh_thread.c mh/streams/mh_file_stream.c mh/mh_context.c mh/collections/mh_shitmap.c mh/collections/mh_collection.c mh/mh_handle.c)
if (WIN32)
    target_link_libraries(mh PUBLIC wsock32 ws2_32 dl)
elseif (HAIKU)
    target_link_libraries(mh PUBLIC pthread bsd)
else(HAIKU)
    target_link_libraries(mh PUBLIC pthread dl)
endif(WIN32)

# mhserv
add_executable(mhserv mhserv.c)
target_link_libraries(mhserv PUBLIC mh)

# libexample.so
add_library(example SHARED example.c)
target_link_libraries(example PUBLIC mh)