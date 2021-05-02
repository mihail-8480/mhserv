cmake_minimum_required(VERSION 3.0)

# mhlib.so
add_subdirectory(src/mh)

project(mhserv C)
include("src/mh/Mh.cmake")

# mhserv
add_executable(mhserv src/mhserv.c)
target_include_directories(mhserv PRIVATE src/mh/include)
target_link_libraries(mhserv PRIVATE mh)

# libexample.so
add_library(example SHARED src/example.c)
target_include_directories(example PRIVATE src/mh/include)
target_link_libraries(example PRIVATE mh)

# tests
add_executable(tests src/tests/tests.c src/tests/simple_tests.c)
target_include_directories(tests PRIVATE src/mh/include)
target_link_libraries(tests PRIVATE mh)