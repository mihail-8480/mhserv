cmake_minimum_required(VERSION 3.0)
project(mhserv C)

if(UNIX AND NOT (APPLE OR HAIKU))
    add_compile_definitions(UNIX_LIKE)
    set(UNIX_LIKE 1)
endif(UNIX AND NOT (APPLE OR HAIKU))

if (WIN32)
    add_compile_definitions(WIN32)
endif (WIN32)

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
add_library(mh SHARED src/mh/streams/mh_stream.c src/mh/streams/mh_memory_stream.c src/mh/mh_memory.c src/mh/network/mh_tcp.c src/mh/streams/mh_socket_stream.c src/mh/network/mh_http.c src/mh/mh_thread.c src/mh/streams/mh_file_stream.c src/mh/mh_context.c src/mh/collections/mh_shitmap.c src/mh/collections/mh_collection.c src/mh/mh_handle.c)

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
add_executable(mhserv src/mhserv.c)
target_link_libraries(mhserv PRIVATE mh)

# libexample.so
add_library(example SHARED src/example.c)
target_link_libraries(example PRIVATE mh)

# tests
add_executable(tests src/tests/tests.c src/tests/simple_tests.c)
target_link_libraries(tests PRIVATE mh)