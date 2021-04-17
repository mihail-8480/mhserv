cmake_minimum_required(VERSION 3.0)
project(mhserv C)

if(UNIX AND NOT (APPLE OR HAIKU))
    add_compile_definitions(UNIX_LIKE)
    set(UNIX_LIKE 1)
endif(UNIX AND NOT (APPLE OR HAIKU))

if (UNIX)
    add_compile_definitions(UNIX)
endif(UNIX)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_compile_definitions(LINUX)
    set(LINUX 1)
endif(CMAKE_SYSTEM_NAME STREQUAL "Linux")

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
    target_link_libraries(mh PRIVATE bsd network)
endif(HAIKU)

if(UNIX_LIKE)
    target_link_libraries(mh PRIVATE pthread dl)
endif(UNIX_LIKE)

# mhserv
add_executable(mhserv mhserv.c)
target_link_libraries(mhserv PRIVATE mh)

# libexample.so
add_library(example SHARED example.c)
target_link_libraries(example PRIVATE mh)

# tests
add_executable(tests tests/tests.c tests/simple_tests.c)
target_link_libraries(tests PRIVATE mh)