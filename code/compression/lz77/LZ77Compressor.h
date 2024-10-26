#pragma once

#include <string>

#include "ICompressor.h"


class LZ77Compressor : public ICompressor
{
private:

    struct _LZ77Match
    {
        int _Offset;
        int _Length;
        char _NextChar;
    };

private:

    static const int WINDOW_SIZE    = 4096; // Size of the search buffer (sliding window)
    static const int LOOKAHEAD_SIZE = 1024; // Size of the lookahead buffer

public:

    void compress(const std::string& inputFilePath, const std::string& outputFilePath) override;
    void decompress(const std::string& inputFilePath, const std::string& outputFilePath) override;
};  // END LZ77Compressor