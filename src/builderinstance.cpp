#include "anagram.h"
#include "builderinstance.h"
#include <algorithm>

void BuilderInstance::generateAnagrams()
{
    AnagramBuilder::Candidate stem;
    AnagramBuilder::WordData target_data;
    target_data.first = target_;
    target_data.second = CharMap(target_);
    // This logic is almost identical to BuildAnagrams but on the
    // outermost loop only a split of the dictionary is iterated through.
    for (const auto& new_word: dict_split_)
    {
        tryAddNewWord(
            new_word,
            target_data,
            stem,
            0);
    }
}

void BuilderInstance::tryAddNewWord(
    const AnagramBuilder::WordData& new_word,
    const AnagramBuilder::WordData& target_data,
    AnagramBuilder::Candidate& stem,
    size_t cur_len)
{
    // Add new words recursively
    if (new_word.first.length() + cur_len <= target_data.first.length())
    {
        // This word is short enough
        auto new_stem = stem;
        new_stem.first.push_back(new_word.first);
        new_stem.second.Append(new_word.second);
        if (target_data.second.Contains(new_stem.second))
        {
            // This word fits into anagram. Try new words recursively.
            cur_len += new_word.first.length();
            buildMoreAnagramsRecursively(target_data, new_stem, cur_len);
        }
    }
}

// Recursive worker function that works by building a stem which is a
// set of words that fit to the target string. As long as the constraints
// are satisfied it goes through the dictionary and calls TryAddNewWord.
// That function checks if the word fits in the anagram and then calls
// BuildAnagram. These function call each other recursively until the
// recursion terminates if stem matches the target exactly or there are
// too many words in the stem.
void BuilderInstance::buildMoreAnagramsRecursively(
    const AnagramBuilder::WordData& target,
    AnagramBuilder::Candidate stem,
    size_t length)
{
    if (stem.second == target.second)
    {
        // Anagram found. Avoid duplicates by sorting.
        sort(stem.first.begin(), stem.first.end());
        std::lock_guard<std::mutex> guard(results_guard_);
        results_.insert(stem.first);
    }
    if (stem.first.size() > max_words_ - 1)
    {
        // Too many words in anagram
        return;
    }
    for (const auto& new_word: dictionary_)
    {
        tryAddNewWord(new_word, target, stem, length);
    }
}