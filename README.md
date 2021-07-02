# Anagram

Command line program written in C++ that prints anagrams of a given words. Uses a dictionary of whitespace-separated words in "words.txt". Dictionary is not supplied.

## Usage

    ./anagram mynamewithoutspaces

## Building

Requires a C++20 compliant compiler such as gcc or clang and cmake (3.16).

    mkdir build
    cd build
    cmake ..
    make

## Testing

Unit tests are a separate CMake project in the `test` subdirectory.

    cd test
    mkdir build
    cd build
    cmake ..
    make
    ./anagramtest

