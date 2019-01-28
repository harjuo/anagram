#include "anagram.h"
#include <fstream>
#include <climits>
#include <algorithm>

CharMap::CharMap() : container(UCHAR_MAX + 1)
{
}

CharMap::CharMap(const std::string& word) : container(UCHAR_MAX + 1)
{
    // Add data from word
    for (auto c: word)
    {
        this->container[c]++;
    }
}

void CharMap::Append(const CharMap& other)
{
    // Add values from other
    for (unsigned int i = 0; i <= UCHAR_MAX; i++)
    {
        this->container[i] += other.container[i];
    }
}

bool CharMap::Contains(const CharMap& other) const
{
    for (unsigned int i = 0; i <= UCHAR_MAX; i++)
    {
        if (this->container[i] < other.container[i])
        {
            // Too many instances of this character in other
            return false;
        }
    }
    // All checks passed, other can fit into this
    return true;
}

bool CharMap::operator==(const CharMap& other) const
{
    for (unsigned int i = 0; i <= UCHAR_MAX; i++)
    {
        if (this->container[i] != other.container[i])
        {
            return false;
        }
    }
    return true;
}

Words ReadWordsFromFile(const std::string& filename, unsigned int min_len)
{
    Words words;
    std::ifstream file;
    file.open(filename);
    std::string word;
    while (file >> word)
    {
        if (word.length() >= min_len)
        {
            words.push_back(word);
        }
    }
    return words;
}

WordDataSet MapWords(const Words& words)
{
    WordDataSet word_data_set;
    for (auto word: words)
    {
        WordData word_data;
        word_data.first = word;
        word_data.second = CharMap(word);
        word_data_set.push_back(word_data);
    }
    return word_data_set;
}

// Main interface function
std::set<Words> Anagrams(const std::string& target,
                         const WordDataSet& dictionary,
                         unsigned int max_words)
{
    std::set<Words> results;
    if (max_words > 0)
    {
        Candidate stem;
        WordData target_data;
        target_data.first = target;
        target_data.second = CharMap(target);
        BuildAnagrams(target_data, dictionary, stem, results, 0, max_words);
    }
    return results;
}

// Recursive worker function that works by building a stem which is a
// set of words that fit to the target string. As long as the constraints
// are satisfied it goes through the dictionary and calls itself recursively
// for each word that would still fit in the stem to match the target word.
// Recursion terminates if stem matches the target exactly or there are
// too many words in the stem.
void BuildAnagrams(const WordData& target,
                   const WordDataSet& dictionary,
                   Candidate stem,
                   std::set<Words>& results,
                   unsigned int length,
                   unsigned int max_words)
{
    if (stem.second == target.second)
    {
        // Anagram found. Sort words to prevent duplicates.
        std::sort(stem.first.begin(), stem.first.end());
        results.insert(stem.first);
    }
    if (stem.first.size() > max_words - 1)
    {
        // Too many words in anagram
        return;
    }
    const auto target_length = target.first.length();
    for (const auto& new_word: dictionary)
    {
        // Add new words recursively
        if (new_word.first.length() + length <= target_length)
        {
            // This word is short enough
            auto new_stem = stem;
            new_stem.first.push_back(new_word.first);
            new_stem.second.Append(new_word.second);
            if (target.second.Contains(new_stem.second))
            {
                // This word fits into anagram. Try new words
                // recursively.
                BuildAnagrams(target, dictionary, new_stem, results,
                              new_word.first.length() + length,
                              max_words);
            }
        }
    }
}