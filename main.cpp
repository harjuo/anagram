#include <string>
#include <iostream>
#include "anagram.h"

const auto MIN_WORD_LEN_LIMITER = 4;
const auto MAX_WORD_COUNT = 3;

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        return 1;
    }
    std::string target = argv[1];
    // The minimum word length depends on the target word
    auto min_word_len = target.length()/MIN_WORD_LEN_LIMITER;
    auto file_words = ReadWordsFromFile("words.txt", min_word_len);
    auto all_words_data = MapWords(file_words);

    auto results = Anagrams(argv[1], all_words_data, MAX_WORD_COUNT);
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
