#pragma once

#include <string>
#include <memory>

#include "ICompressor.h"


enum class ECompressor
{
    e_HUFFMAN,
    e_LZ77
};  // END ECompressor


class CompressorFacade
{
public:

    void compress(const std::string& inputFilePath, const std::string& outputFilePath, ECompressor type);
    void decompress(const std::string& inputFilePath, const std::string& outputFilePath, ECompressor type);

private:

    std::unique_ptr<ICompressor> _get_compressor(ECompressor type); // factory method
};  // END CompressorFacade