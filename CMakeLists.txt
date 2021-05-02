cmake_minimum_required(VERSION 3.0)

project(mhserv C)
include("Mh.cmake")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,-R/usr/local/lib ${EXTRA_FLAGS}")

# mhserv
add_executable(mhserv src/mhserv.c)
target_link_libraries(mhserv PRIVATE mh)

# libexample.so
add_library(example SHARED src/example.c)
target_link_libraries(example PRIVATE mh)

# tests
add_executable(tests src/tests/tests.c src/tests/simple_tests.c)
target_link_libraries(tests PRIVATE mh)

# Install
install(TARGETS mhserv
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)