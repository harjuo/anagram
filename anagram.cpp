#include "anagram.h"
#include <fstream>

typedef std::map<char, unsigned int> Breakdown;
typedef std::pair<std::string, Breakdown> WordData;
typedef std::set<WordData> WordDataSet;
typedef std::set<std::string> Words;
typedef std::pair<Words, Breakdown> Candidate;

void buildAnagrams(const WordData& target, 
							  const WordDataSet& dictonary,
							  Candidate stem,
							  std::set<Words>& results,
							  unsigned int length,
							  unsigned int max_words);

Words readWordsFromFile(const std::string& filename, unsigned int min_len)
{
	Words words;
	std::ifstream file;	
	file.open(filename);
	std::string word;
	while (file >> word)
	{
		if (word.length() >= min_len)
		{
			words.insert(word);
		}
    }
	return words;	
}

Breakdown breakWord(const std::string& word)
{
	Breakdown bw;
	for (auto c: word) 
	{
		bw[c]++;
	}
	return bw;
}

WordDataSet breakAllWords(const Words& words)
{
	WordDataSet word_data_set;
	for (auto word: words)
	{
		WordData word_data;
		word_data.first = word;
		word_data.second = breakWord(word);
		word_data_set.insert(word_data);
	}
	return word_data_set;
}

void breakdownAppend(Breakdown& target, const Breakdown& other)
{
	for(const auto& other_unit: other) 
	{
		target[other_unit.first] += other_unit.second;
	}
}

bool fitsInto(const Breakdown& bw_target, const Breakdown& bw_cand)
{
	for (const auto& bc: bw_cand)
	{
		const auto& bt = bw_target.find(bc.first);
		if (bt != bw_target.end())
		{
			if (bt->second < bc.second)
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	return true;
}

std::set<Words> anagrams(const std::string& target, 
						 const WordDataSet& dictionary,
						 unsigned int max_words)
{
	Candidate stem;
	std::set<Words> results;
	WordData target_data; 
	target_data.first = target;
	target_data.second = breakWord(target);
	buildAnagrams(target_data, dictionary, stem, results, 0, max_words); 
	return results;
}

void buildAnagrams(const WordData& target, 
							  const WordDataSet& dictionary,
							  Candidate stem,
							  std::set<Words>& results,
							  unsigned int length,
							  unsigned int max_words)
{
	if (stem.second == target.second)
	{
		// Anagram found
		results.insert(stem.first);
	}
	if (stem.first.size() > max_words - 1)
	{
		// Too many words in anagram
		return;
	}
	for (const auto& new_word: dictionary)
	{
		// Add new words recursively
		if (new_word.first.length() + length <= target.first.length())
		{
			// This word is short enough
			Candidate new_stem = stem;
			new_stem.first.insert(new_word.first);
			breakdownAppend(new_stem.second, new_word.second);
			if(fitsInto(target.second, new_stem.second))
			{
				// This word fits into anagram. Try new words
				// recursively.
				buildAnagrams(target, dictionary, new_stem, results, 
						      new_word.first.length() + length,
							  max_words);	
			}
		}
	}
}