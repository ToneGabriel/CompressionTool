#pragma once

#include <string>

class ICompressor
{
public:

    virtual void compress(const std::string& path) = 0;
    virtual void decompress(const std::string& path) = 0;

    ~ICompressor() = default;
};  // END ICompressor