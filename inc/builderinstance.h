#ifndef BUILDERINSTANCE_H
#define BUILDERINSTANCE_H

#include "anagram.h"
#include <string>
#include <set>
#include <mutex>

class BuilderInstance
{
public:
    BuilderInstance(
        const WordDataSet& dict_split,
        const WordDataSet& dictionary,
        const std::string& target,
        std::set<Words>& results,
        std::mutex& results_guard,
        size_t max_words
    ) :
    dict_split_(dict_split),
    dictionary_(dictionary),
    target_(target),
    results_(results),
    results_guard_(results_guard),
    max_words_(max_words)
    {
    }

    // TODO can't do this yet BuilderInstance(const BuilderInstance&) = delete;
    BuilderInstance& operator=(const BuilderInstance&) = delete;

    void generateAnagrams();

private:
    void tryAddNewWord(
        const WordData& new_word,
        const WordData& target_data,
        Candidate& stem,
        size_t cur_len);

    void buildMoreAnagramsRecursively(
        const WordData& target,
        Candidate stem,
        size_t length);

    const WordDataSet& dict_split_;
    const WordDataSet& dictionary_;
    const std::string& target_;
    std::set<Words>& results_;
    std::mutex& results_guard_;
    size_t max_words_;
};

#endif