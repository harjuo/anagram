project(anagramtest)
add_executable(anagramtest test.cpp ../anagram.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall -g -Wextra -std=c++11")

