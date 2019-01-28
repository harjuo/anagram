#ifndef ANAGRAM_H
#define ANAGRAM_H

#include <map>
#include <set>
#include <utility>
#include <vector>

// Maps strings into character counts.
class CharMap
{
public:
    CharMap();
    CharMap(const std::string& word);

    // Adds the data from another map to this
    void Append(const CharMap& other);

    // Returns true if this one has at least one character for
    // each character in the other map.
    bool Contains(const CharMap& other) const;

    bool operator==(const CharMap& other) const;

    // Used by standard containers, returns std::map<T>::operator<
    bool operator<(const CharMap& other) const;

private:
    std::map<char, unsigned int> container;
};

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
                   unsigned int length,
                   unsigned int max_words);

// Populates words from words.txt
Words ReadWordsFromFile(const std::string& filename, unsigned int min_len);

// Populates character map for a set of words
WordDataSet MapWords(const Words& words);

// Main interface function that returns a set of anagrams for a target
// given a dictionary of words that have been processed to character maps.
// Will return anagrams containing up to max_words number of words.
std::set<Words> Anagrams(const std::string& target,
                         const WordDataSet& dictionary,
                         unsigned int max_words);

#endif