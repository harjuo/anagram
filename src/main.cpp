#include <string>
#include <iostream>
#include "anagram.h"

const auto MIN_WORD_LEN = 2;
const auto MAX_WORD_COUNT = 2;

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        return 1;
    }
    std::string target = argv[1];
    auto anagramBuilder = AnagramBuilder(target, "words.txt", MIN_WORD_LEN, MAX_WORD_COUNT);
    for (const auto& words: anagramBuilder.getAnagrams())
    {
        for (const auto& word: words)
        {
            std::cout << word << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}
