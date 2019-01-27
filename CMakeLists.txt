project(anagram)
add_executable(anagram anagram.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall -O3 -Wextra -std=c++11")

