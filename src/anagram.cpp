#include "anagram.h"
#include "builderinstance.h"
#include "charmap.h"
#include <fstream>
#include <thread>
#include <memory>

typedef std::vector<std::unique_ptr<std::thread>> thread_vector;

AnagramBuilder::AnagramBuilder(
        const std::string& target,
        const std::string& filename,
        size_t min_len,
        size_t max_words
) : 
    target_(target),
    filename_(filename),
    min_len_(min_len),
    max_words_(max_words)
{
}

std::set<AnagramBuilder::Words> AnagramBuilder::getAnagrams()
{
    auto file_words = readWordsFromFile();
    auto all_words_data = mapWords(file_words);
    auto results = anagrams(all_words_data);
    return results;
}

AnagramBuilder::Words AnagramBuilder::readWordsFromFile()
{
    Words words;
    std::ifstream file;
    file.open(filename_);
    std::string word;
    while (file >> word)
    {
        if (word.length() >= min_len_)
        {
            words.push_back(word);
        }
    }
    return words;
}

AnagramBuilder::WordDataSet AnagramBuilder::mapWords(const Words& words)
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
void AnagramBuilder::splitWork(std::vector<AnagramBuilder::WordDataSet>& dict_splits,
    const AnagramBuilder::WordDataSet& dictionary)
{
    size_t i = 0;
    for (; i < MAX_NUM_THREADS; i++)
    {
        dict_splits.push_back(AnagramBuilder::WordDataSet());
    }
    i = 0;
    for (const auto& word: dictionary)
    {
        dict_splits.at(i % MAX_NUM_THREADS).push_back(word);
        i++;
    }
}

static void threadWorker(BuilderInstance& ctx)
{
    ctx.generateAnagrams();
}

// Main interface function. Divides the computation to several threads
// and collects results from them.
std::set<AnagramBuilder::Words> AnagramBuilder::anagrams(WordDataSet& dictionary)
{
    std::set<Words> results;
    if (max_words_ < 1)
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
    splitWork(dict_splits, dictionary);

    std::mutex results_guard;

    // Create contexes for threads
    //for (size_t thread_num = 0; thread_num < MAX_NUM_THREADS; thread_num++)
    for (size_t thread_num = 0; thread_num < MAX_NUM_THREADS; ++thread_num)
    {
        auto ctx = BuilderInstance(
            dict_splits.at(thread_num),
            dictionary,
            target_,
            thread_results.at(thread_num),
            results_guard,
            max_words_
        );

        thread_contexes.push_back(ctx);
    }

    // Start all the threads
    for (size_t thread_num = 0; thread_num < MAX_NUM_THREADS; ++thread_num)
    {
        worker_threads.at(thread_num) = std::make_unique<std::thread>(
            std::thread(
                threadWorker,
                std::ref(thread_contexes.at(thread_num))));
    }

    // Join the threads
    for (size_t thread_num = 0; thread_num < MAX_NUM_THREADS; ++thread_num)
    {
        worker_threads.at(thread_num)->join();
    }

    // Collect results from threads
    for (size_t thread_num = 0; thread_num < MAX_NUM_THREADS; ++thread_num)
    {
        results.insert(thread_results.at(thread_num).begin(),
                       thread_results.at(thread_num).end());
    }

    return results;
}
