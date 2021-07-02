#include "anagram.h"
#include "builderinstance.h"
#include "charmap.h"
#include <fstream>
#include <thread>
#include <memory>

typedef std::vector<std::unique_ptr<std::thread>> thread_vector;

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


static void ThreadWorker(BuilderInstance& ctx)
{
    ctx.generateAnagrams();
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
    std::vector<BuilderInstance> thread_contexes;
    // Each thread has a split of dictionary to go through
    std::vector<WordDataSet> dict_splits;
    // Each thread has its own set of results
    std::vector<std::set<Words>> thread_results(MAX_NUM_THREADS);
    SplitWork(dict_splits, dictionary);

    std::mutex results_guard;

    // Create contexes for threads
    //for (size_t thread_num = 0; thread_num < MAX_NUM_THREADS; thread_num++)
    for (const auto &thread_num: thread_ids)
    {
        auto ctx = BuilderInstance(
            dict_splits.at(thread_num),
            dictionary,
            target,
            thread_results.at(thread_num),
            results_guard,
            max_words
        );

        thread_contexes.push_back(ctx);
    }

    // Start all the threads
    for (const auto &thread_num: thread_ids)
    {
        worker_threads.at(thread_num) = std::make_unique<std::thread>(
            std::thread(
                ThreadWorker,
                std::ref(thread_contexes.at(thread_num))));
    }

    // Join the threads
    for (const auto &thread_num: thread_ids)
    {
        worker_threads.at(thread_num)->join();
    }

    // Collect results from threads
    for (const auto &thread_num: thread_ids)
    {
        results.insert(thread_results.at(thread_num).begin(),
                       thread_results.at(thread_num).end());
    }

    return results;
}
