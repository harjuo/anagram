#include <string>
#include <iostream>
#include "anagram.h"

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        return 1;
    }
    std::string target = argv[1];
    Words words = readWordsFromFile("words.txt", target.length()/4);
    WordDataSet all_words_data = breakAllWords(words);
    std::set<Words> results = anagrams(argv[1], all_words_data, target.length()/3);
    for (const auto& words: results)
    {
        for (const auto& word: words)
        {
            std::cout << word << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}
