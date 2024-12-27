cmake_minimum_required(VERSION 3.29)
project(itoq)

set(CMAKE_CXX_STANDARD 20)

add_executable(tests test.cpp pribil.cpp)
add_executable(main main.cpp pribil.cpp
        doctest.h)
