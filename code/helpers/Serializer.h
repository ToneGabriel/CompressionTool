#pragma once

#include <fstream>
#include <string>
#include <map>

#include "_Core.h"

// is any of
template<class Ty, class... Types>              // true if Ty is in Types
constexpr bool Is_Any_Of_v = std::disjunction_v<std::is_same<Ty, Types>...>;

template<class Ty>
struct Is_Any_Of : std::bool_constant<Is_Any_Of_v<Ty>> {};

// is serializable
template<class Type>
constexpr bool Is_Serializable_v = Is_Any_Of_v<std::remove_cv_t<Type>,
                                                    char,
                                                    int // TODO
                                                >;


template<class Type>
void serialize(const Type& target, std::ofstream& ofile)
{
    static_assert(Is_Serializable_v<Type>, "Type not serializable");
    _ASSERT(ofile.is_open(), "File not open!");

    ofile.write(reinterpret_cast<const char*>(&target), sizeof(Type));
}

template<class Type>
Type deserialize(std::ifstream& ifile)
{
    static_assert(Is_Serializable_v<Type>, "Type not serializable");
    _ASSERT(ifile.is_open(), "File not open!");

    Type ret;
    ifile.read(reinterpret_cast<char*>(&ret), sizeof(Type));
    return ret;
}