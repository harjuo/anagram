#ifndef ANAGRAM_H
#define ANAGRAM_H

#include "charmap.h"
#include <map>
#include <set>
#include <utility>
#include <vector>
#include <mutex>

const auto MAX_NUM_THREADS = 16;
const unsigned int thread_ids[MAX_NUM_THREADS] =
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

// Pairs a word's string representation and character map
typedef std::pair<std::string, CharMap> WordData;

// Collection of multiple word data
typedef std::vector<WordData> WordDataSet;

// Collection of words
typedef std::vector<std::string> Words;

// Map of several words and their string representation
typedef std::pair<Words, CharMap> Candidate;

// Recursive function to build anagrams.
void BuildAnagrams(const WordData& target,
                   const WordDataSet& dictonary,
                   Candidate stem,
                   std::set<Words>& results,
                   size_t length,
                   size_t max_words,
                   std::mutex& results_guard);

// Populates words from words.txt
Words ReadWordsFromFile(const std::string& filename, size_t min_len);

// Populates character map for a set of words
WordDataSet MapWords(const Words& words);

// Main interface function that returns a set of anagrams for a target
// given a dictionary of words that have been processed to character maps.
// Will return anagrams containing up to max_words number of words.
std::set<Words> Anagrams(const std::string& target,
                         WordDataSet& dictionary,
                         size_t max_words);

#endif
