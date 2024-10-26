#pragma once

#include <string>

class ICompressor
{
public:

    virtual void compress(const std::string& inputFilePath, const std::string& outputFilePath) = 0;
    virtual void decompress(const std::string& inputFilePath, const std::string& outputFilePath) = 0;

    ~ICompressor() = default;
};  // END ICompressor