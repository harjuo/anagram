#ifndef ANAGRAM_H
#define ANAGRAM_H

#include "charmap.h"
#include <map>
#include <set>
#include <utility>
#include <vector>
#include <mutex>

const auto MAX_NUM_THREADS = 16;

class AnagramBuilder {
public:
    // Collection of words
    typedef std::vector<std::string> Words;

    // Pairs a word's string representation and character map
    typedef std::pair<std::string, CharMap> WordData;

    // Collection of multiple word data
    typedef std::vector<WordData> WordDataSet;

    // Map of several words and their string representation
    typedef std::pair<Words, CharMap> Candidate;

    AnagramBuilder(
        const std::string& target,
        const std::string& filename,
        size_t min_len,
        size_t max_words
    );

    AnagramBuilder(const AnagramBuilder&) = delete;
    AnagramBuilder& operator=(const AnagramBuilder&) = delete;

    std::set<Words> getAnagrams();

private:
    // Populates words from given file
    Words readWordsFromFile();

    // Populates character map for a set of words
    WordDataSet mapWords(const Words& words);

    // Main worker function that returns a set of anagrams for a target
    // given a dictionary of words that have been processed to character maps.
    // Will return anagrams containing up to max_words number of words.
    std::set<Words> anagrams(WordDataSet& dictionary);

    void splitWork(
        std::vector<AnagramBuilder::WordDataSet>& dict_splits,
        const AnagramBuilder::WordDataSet& dictionary);

    const std::string& target_;
    const std::string& filename_;
    size_t min_len_;
    size_t max_words_;
};

#endif
