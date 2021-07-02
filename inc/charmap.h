#ifndef CHARMAP_H
#define CHARMAP_H

#include <map>
#include <string>

// Maps strings into character counts.
class CharMap
{
public:
    CharMap();
    CharMap(const std::string& word);

    // Adds the data from another map to this
    void Append(const CharMap& other);

    // Returns true if this one has at least one character for
    // each character in the other map.
    bool Contains(const CharMap& other) const;

    bool operator==(const CharMap& other) const;

    // Used by standard containers, returns std::map<T>::operator<
    bool operator<(const CharMap& other) const;

private:
    std::map<char, unsigned int> container;
};

#endif // CHARMAP_H