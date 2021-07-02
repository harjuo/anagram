#include "charmap.h"

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
    for (const auto& other_unit: other.container)
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
