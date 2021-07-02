#include "anagram.h"
#include <cassert>
#include <fstream>
#include <iostream>

int main()
{
    std::ofstream wordFile;
    wordFile.open("words.txt");
    wordFile << "foo\n";
    wordFile << "bar\n";
    wordFile << "baz\n";
    wordFile << "barbaz\n";
    wordFile << "foobar\n";
    wordFile.close();
    auto anagramBuilder = AnagramBuilder("babaforzo", "words.txt", 0, 100);
    auto results = anagramBuilder.getAnagrams();
    assert(results.size() == 3);
    std::cout << "Passed" << std::endl;
    return 0;
}
