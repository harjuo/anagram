#include "anagram.h"
#include <fstream>
#include <algorithm>
#include <thread>
#include <memory>

typedef std::vector<std::unique_ptr<std::thread>> thread_vector;

CharMap::CharMap() : container()
{
}

CharMap::CharMap(const std::string& word)
{
    for (auto c: word)
    {
        this->container[c]++;
    }
}

void CharMap::Append(const CharMap& other)
{
    for(const auto& other_unit: other.container)
    {
        this->container[other_unit.first] += other_unit.second;
    }
}

bool CharMap::Contains(const CharMap& other) const
{
    for (const auto& other_char: other.container)
    {
        const auto& this_char = this->container.find(other_char.first);
        if (this_char != this->container.end())
        {
            if (this_char->second < other_char.second)
            {
                // Too many characters in other
                return false;
            }
        }
        else
        {
            // Character not found in this container
            return false;
        }
    }
    // All checks passed, other can fit into this
    return true;
}

bool CharMap::operator==(const CharMap& other) const
{
    return this->container == other.container;
}

bool CharMap::operator<(const CharMap& other) const
{
    return this->container < other.container;
}

Words ReadWordsFromFile(const std::string& filename, size_t min_len)
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

struct ThreadContext
{
    const WordDataSet& dict_split;
    const WordDataSet& dictionary;
    const std::string& target;
    std::set<Words>& results;
    std::mutex& results_guard;
    size_t max_words;
};

// Encapsulates a thread and results of its computation
struct ThreadContainer
{
    std::thread* thread;
    std::set<Words>* results;
};

// Splits dictionary to smaller parts that can be assigned to separate threads
static void SplitWork(std::vector<WordDataSet>& dict_splits,
               const WordDataSet& dictionary)
{
    size_t i = 0;
    for (; i < MAX_NUM_THREADS; i++)
    {
        dict_splits.push_back(WordDataSet());
    }
    i = 0;
    for (const auto& word: dictionary)
    {
        dict_splits.at(i % MAX_NUM_THREADS).push_back(word);
        i++;
    }
}

static void TryAddNewWord(const WordData& new_word,
                   const WordData& target_data,
                   const WordDataSet& dictionary,
                   Candidate& stem,
                   std::set<Words>& results,
                   size_t cur_len,
                   size_t max_words,
                   std::mutex& results_guard)
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
            BuildAnagrams(
                target_data,
                dictionary,
                new_stem,
                results,
                cur_len,
                max_words,
                results_guard);
        }
    }
}

static void ThreadWorker(ThreadContext& ctx)
{
    Candidate stem;
    WordData target_data;
    target_data.first = ctx.target;
    target_data.second = CharMap(ctx.target);
    // This logic is almost identical to BuildAnagrams but on the
    // outermost loop only a split of the dictionary is iterated through.
    for (const auto& new_word: ctx.dict_split)
    {
        TryAddNewWord(
            new_word,
            target_data,
            ctx.dictionary,
            stem,
            ctx.results,
            0,
            ctx.max_words,
            ctx.results_guard);
    }
}


// Main interface function. Divides the computation to several threads
// and collects results from them.
std::set<Words> Anagrams(const std::string& target,
                         WordDataSet& dictionary,
                         size_t max_words)
{
    std::set<Words> results;
    if (max_words < 1)
    {
        // Return empty results
        return results;
    }

    // Split the work to MAX_NUM_THREADS
    thread_vector worker_threads(MAX_NUM_THREADS);
    // Each thread has its own context
    std::vector<ThreadContext> thread_contexes;
    // Each thread has a split of dictionary to go through
    std::vector<WordDataSet> dict_splits;
    // Each thread has its own set of results
    std::vector<std::set<Words>> thread_results(MAX_NUM_THREADS);
    SplitWork(dict_splits, dictionary);

    std::mutex results_guard;

    // Create contexes for threads
    for (size_t thread_num = 0; thread_num < MAX_NUM_THREADS; thread_num++)
    {
        ThreadContext ctx = {
            .dict_split = dict_splits.at(thread_num),
            .dictionary = dictionary,
            .target = target,
            .results = thread_results.at(thread_num),
            .results_guard = results_guard,
            .max_words = max_words
        };
        thread_contexes.push_back(ctx);
    }

    // Start all the threads
    for (size_t thread_num = 0; thread_num < MAX_NUM_THREADS; thread_num++)
    {
        worker_threads.at(thread_num) = std::make_unique<std::thread>(
            std::thread(
                ThreadWorker,
                std::ref(thread_contexes.at(thread_num))));
    }

    // Join the threads
    for(size_t i = 0; i < MAX_NUM_THREADS; i++)
    {
        worker_threads.at(i)->join();
    }

    // Collect results from threads
    for(size_t i = 0; i < MAX_NUM_THREADS; i++)
    {
        results.insert(thread_results.at(i).begin(),
                       thread_results.at(i).end());
    }

    return results;
}

// Recursive worker function that works by building a stem which is a
// set of words that fit to the target string. As long as the constraints
// are satisfied it goes through the dictionary and calls TryAddNewWord.
// That function checks if the word fits in the anagram and then calls
// BuildAnagram. These function call each other recursively until the
// recursion terminates if stem matches the target exactly or there are
// too many words in the stem.
void BuildAnagrams(const WordData& target,
                   const WordDataSet& dictionary,
                   Candidate stem,
                   std::set<Words>& results,
                   size_t length,
                   size_t max_words,
                   std::mutex& results_guard)
{
    if (stem.second == target.second)
    {
        // Anagram found. Avoid duplicates by sorting.
        sort(stem.first.begin(), stem.first.end());
        std::lock_guard<std::mutex> guard(results_guard);
        results.insert(stem.first);
    }
    if (stem.first.size() > max_words - 1)
    {
        // Too many words in anagram
        return;
    }
    for (const auto& new_word: dictionary)
    {
        TryAddNewWord(new_word, target, dictionary, stem, results,
                      length, max_words, results_guard);
    }
}
