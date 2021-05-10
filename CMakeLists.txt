cmake_minimum_required(VERSION 3.0)

project(mhserv C)
include("Mh.cmake")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_FLAGS}")

# mhserv
add_executable(mhserv src/mhserv.c)
target_link_libraries(mhserv PRIVATE mh)

# libexample.so
add_library(example MODULE src/example.c)
target_link_libraries(example PRIVATE mh)

# Install
install(TARGETS mhserv
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)