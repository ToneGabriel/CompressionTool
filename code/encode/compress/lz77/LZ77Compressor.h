#pragma once

#include <string>

#include "IEncoder.h"


class LZ77Compressor : public IEncoder
{
private:

    struct _LZ77Match
    {
        int _Offset;
        int _Length;
        char _NextChar;
    };  // END _LZ77Match

private:

    static const int WINDOW_SIZE    = 4096; // Size of the search buffer (sliding window)
    static const int LOOKAHEAD_SIZE = 1024; // Size of the lookahead buffer

public:

    LZ77Compressor() = default;
    ~LZ77Compressor() = default;

public:

    void encode(const std::string& inputFilePath, const std::string& outputFilePath) override;
    void decode(const std::string& inputFilePath, const std::string& outputFilePath) override;
};  // END LZ77Compressor