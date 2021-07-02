cmake_minimum_required(VERSION 3.16)

project(anagram)

add_executable(
    anagram
    src/main.cpp
    src/anagram.cpp
    src/charmap.cpp
    src/builderinstance.cpp
)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall -O3 -Wextra\
    -pedantic -pedantic-errors -std=c++20")

target_include_directories(anagram PRIVATE ${CMAKE_SOURCE_DIR}/inc)
