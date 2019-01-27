# Anagram

Command line program written in C++ that prints anagrams of a given words. Uses a dictionary of whitespace-separated words in "words.txt". Dictionary is not supplied.

## Usage

    ./anagram mynamewithoutspaces

## Building

Requires a C++11 compliant g++ or clang.

    cmake .
    make

## Testing

Unit tests are a separate CMake project in the `test` subdirectory.

    cd test
    cmake .
    make
    ./anagramtest
