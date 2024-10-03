#pragma once

#include <cstddef>      // size_t, nullptr_t
#include <stdexcept>    // exceptions
#include <climits>      // numeric limits
#include <iostream>     // debugging

#include "Config.hpp"   // project configuration


inline void __Assert(bool expr, const char* msg, const char* expected, const char* file, int line)
{
    if (!expr)
    {
        std::cerr   << "Assert failed:\t"   << msg      << "\n"
                    << "Expected:\t"        << expected  << "\n"
                    << "File:\t\t"          << file     << "\n"
                    << "Line:\t\t"          << line     << "\n";
        ::abort();
    }
    else
    {
        // assert OK - do nothing
    }
}

#define _ASSERT(Expr, Msg) __Assert(Expr, Msg, #Expr, __FILE__, __LINE__)

#define SYMBOL_BIT          CHAR_BIT
#define SYMBOL_ZERO         '0'
#define SYMBOL_ONE          '1'

#define EXTENSION_SIZE      10
#define HUFFMAN_EXTENSION   ".hff"
#define LZ77_EXTENSION      ".lz77"

using symbol_t = char;