#include "anagram.h"
#include "charmap.h"
#include <iostream>
#include <cassert>

void populate_dictionary(Words& dictionary)
{
    dictionary.push_back("foo");
    dictionary.push_back("bar");
    dictionary.push_back("baz");
    dictionary.push_back("barbaz");
    dictionary.push_back("foobar");
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

void test_scandic_anagrams()
{
    Words dictionary;
    dictionary.push_back("pässi");
    dictionary.push_back("pössi");
    dictionary.push_back("påssi");
    dictionary.push_back("passi");

    auto mapped_words = MapWords(dictionary);
    auto max_word_count = 3;
    auto results = Anagrams("pässi", mapped_words, max_word_count);

    assert(results.size() == 1);
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
    test_scandic_anagrams();
    test_no_anagrams();
    test_empty_dict();
    test_zero_max_word_count();
    std::cout << "Passed" << std::endl;
    return 0;
}
