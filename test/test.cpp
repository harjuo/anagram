#include <iostream>
#include <cassert>
#include "../anagram.h"

void populate_dictionary(Words& dictionary)
{
    dictionary.insert("foo");
    dictionary.insert("bar");
    dictionary.insert("baz");
    dictionary.insert("barbaz");
    dictionary.insert("foobar");
}

void test_three_anagrams()
{
    Words dictionary;
    populate_dictionary(dictionary);

    auto mapped_words = MapWords(dictionary);
    auto max_word_count = 3;
    auto results = Anagrams("babaforzo", mapped_words, max_word_count);

    assert(results.size() == 3);
}

void test_no_anagrams()
{
    Words dictionary;
    populate_dictionary(dictionary);

    auto mapped_words = MapWords(dictionary);
    auto max_word_count = 3;
    auto results = Anagrams("qwerty", mapped_words, max_word_count);

    assert(results.size() == 0);
}

void test_empty_dict()
{
    Words dictionary;

    auto mapped_words = MapWords(dictionary);
    auto max_word_count = 3;
    auto results = Anagrams("qwerty", mapped_words, max_word_count);

    assert(results.size() == 0);
}

void test_zero_max_word_count()
{
    Words dictionary;
    populate_dictionary(dictionary);

    auto mapped_words = MapWords(dictionary);
    auto max_word_count = 0;
    auto results = Anagrams("babaforzo", mapped_words, max_word_count);

    assert(results.size() == 0);
}

int main()
{
    test_three_anagrams();
    test_no_anagrams();
    test_empty_dict();
    test_zero_max_word_count();
    std::cout << "Passed" << std::endl;
    return 0;
}
