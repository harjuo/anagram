cmake_minimum_required(VERSION 3.10)
project(anagramtest)
add_executable(
    anagramtest
    test.cpp
    ../src/anagram.cpp
    ../src/charmap.cpp
    ../src/builderinstance.cpp
)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pthread -g -Wextra -pedantic -pedantic-errors -std=c++20")
set(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
set(GCC_COVERAGE_LINK_FLAGS "-lgcov")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
target_include_directories(anagramtest PRIVATE ${CMAKE_SOURCE_DIR}/../inc)
