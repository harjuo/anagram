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
        const AnagramBuilder::WordDataSet& dict_split,
        const AnagramBuilder::WordDataSet& dictionary,
        const std::string& target,
        std::set<AnagramBuilder::Words>& results,
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
        const AnagramBuilder::WordData& new_word,
        const AnagramBuilder::WordData& target_data,
        AnagramBuilder::Candidate& stem,
        size_t cur_len);

    void buildMoreAnagramsRecursively(
        const AnagramBuilder::WordData& target,
        AnagramBuilder::Candidate stem,
        size_t length);

    const AnagramBuilder::WordDataSet& dict_split_;
    const AnagramBuilder::WordDataSet& dictionary_;
    const std::string& target_;
    std::set<AnagramBuilder::Words>& results_;
    std::mutex& results_guard_;
    size_t max_words_;
};

#endif