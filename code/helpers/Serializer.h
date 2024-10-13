#pragma once

#include <fstream>
#include <string>
#include <map>

#include "_Core.h"

// is basic type
template<class Type>
struct Is_Basic_Type : std::disjunction<std::is_integral<Type>,         // Check if integral type (int, char, etc.)
                                        std::is_floating_point<Type>    // Check if floating point type (float, double)
                                        > {};

template<class Type>
constexpr bool Is_Basic_Type_v = Is_Basic_Type<Type>::value;


// serialize basic types
template<class Type>
void serialize(const Type& target, std::ofstream& ofile)
{
    static_assert(Is_Basic_Type_v<Type>, "Type not serializable");
    _ASSERT(ofile.is_open(), "File not open!");

    ofile.write(reinterpret_cast<const char*>(&target), sizeof(Type));
}

template<class Type>
void deserialize(Type& ret, std::ifstream& ifile)
{
    static_assert(Is_Basic_Type_v<Type>, "Type not serializable");
    _ASSERT(ifile.is_open(), "File not open!");

    ifile.read(reinterpret_cast<char*>(&ret), sizeof(Type));
}

// serialize string
void serialize(const std::string& target, std::ofstream& ofile)
{
    _ASSERT(ofile.is_open(), "File not open!");

    size_t targetSize = target.size();
    ofile.write(reinterpret_cast<const char*>(&targetSize), sizeof(size_t));

    ofile.write(reinterpret_cast<const char*>(&target[0]), targetSize);
}

void deserialize(std::string& ret, std::ifstream& ifile)
{
    _ASSERT(ifile.is_open(), "File not open!");

    size_t retSize;
    ifile.read(reinterpret_cast<char*>(&retSize), sizeof(size_t));

    ret.resize(retSize);
    ifile.read(reinterpret_cast<char*>(&ret[0]), retSize);
}

// serialize map
template<class Key, class Value>
void serialize(const std::map<Key, Value>& target, std::ofstream& ofile)
{
    _ASSERT(ofile.is_open(), "File not open!");

    serialize(target.size(), ofile);

    for (const auto& [key, val] : target)
    {
        serialize(key, ofile);
        serialize(val, ofile);
    }
}

template<class Key, class Value>
void deserialize(std::map<Key, Value>& ret, std::ifstream& ifile)
{
    _ASSERT(ifile.is_open(), "File not open!");

    size_t retSize;
    deserialize(retSize, ifile);

    ret.clear();

    Key k;
    Value v;
    for (size_t i = 0; i < retSize; ++i)
    {
        deserialize(k, ifile);
        deserialize(v, ifile);
        ret[k] = v;
    }
}