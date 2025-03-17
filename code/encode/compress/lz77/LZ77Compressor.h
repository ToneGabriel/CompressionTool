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
        bool _HasNextChar;
    };  // END _LZ77Match

private:

    static const int WINDOW_SIZE    = 16 * 1024;    // 16KB - Size of the search buffer (sliding window)
    static const int LOOKAHEAD_SIZE = 4 * 1024;     // 4KB - Size of the lookahead buffer

public:

    LZ77Compressor() = default;
    ~LZ77Compressor() = default;

private:

    void _Encode(const std::string& inputFilePath, const std::string& outputFilePath) override;
    void _Decode(const std::string& inputFilePath, const std::string& outputFilePath) override;
};  // END LZ77Compressor