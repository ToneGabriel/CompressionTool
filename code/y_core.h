#pragma once
#include <cstddef>      // size_t, nullptr_t
#include <stdexcept>    // exceptions
#include <climits>
#include <iostream>     // debugging

#include "Config.hpp"   // project configuration


#define CUSTOM_ASSERT(Expr, Msg)        __Assert(Expr, Msg, #Expr, __FILE__, __LINE__)
#define CUSTOM_TEST_RUN(Count, Params)  __TestRun(Count, Params)

inline void __Assert(bool expr, const char* msg, const char* exprStr, const char* file, int line)
{
    if (!expr)
    {
        std::cerr   << "Assert failed:\t"   << msg << "\n"
                    << "Expected:\t"        << exprStr << "\n"
                    << "Source:\t\t"        << file << ", line " << line << "\n";
        ::abort();
    }
    else
    {
        // expr OK - do nothing
    }
}

inline void __TestRun(int argc, char** args)
{
    std::cout << "Project Name = "          << PROJECT_NAME             << '\n';
    std::cout << "Project Version = "       << PROJECT_VERSION          << '\n';
    std::cout << "Project Version Major = " << PROJECT_VERSION_MAJOR    << '\n';
    std::cout << "Project Version Minor = " << PROJECT_VERSION_MINOR    << '\n';
    std::cout << "Project Version Patch = " << PROJECT_VERSION_PATCH    << '\n';

    std::cout << "Argument Count = "        << argc                     << '\n';
    
    for (int i = 0; i < argc; ++i)
        std::cout << "Argument[" << i << "] = " << args[i] << '\n';
}