#ifndef ANAGRAM_H
#define ANAGRAM_H

#include <map>
#include <set>
#include <utility>

// Maps strings into character counts.
class CharMap
{
public:
    // Constructors and helper methods for a std::map
    CharMap();
    CharMap(const std::string& word);
    void Append(const CharMap& other);
    bool Contains(const CharMap& other) const;
    bool operator==(const CharMap& other) const; 
    bool operator<(const CharMap& other) const;
private:
    std::map<char, unsigned int> container;
};

typedef std::pair<std::string, CharMap> WordData;
typedef std::set<WordData> WordDataSet;
typedef std::set<std::string> Words;
typedef std::pair<Words, CharMap> Candidate;

void buildAnagrams(const WordData& target, 
                              const WordDataSet& dictonary,
                              Candidate stem,
                              std::set<Words>& results,
                              unsigned int length,
                              unsigned int max_words);

Words readWordsFromFile(const std::string& filename, unsigned int min_len);

WordDataSet breakAllWords(const Words& words);

std::set<Words> anagrams(const std::string& target, 
                         const WordDataSet& dictionary,
                         unsigned int max_words);

void buildAnagrams(const WordData& target, 
                              const WordDataSet& dictionary,
                              Candidate stem,
                              std::set<Words>& results,
                              unsigned int length,
                              unsigned int max_words);

#endif