#pragma once

#include <string>
#include <memory>

#include "huffman/HuffmanCompressor.h"
#include "lz77/LZ77Compressor.h"


enum class ECompressor
{
    e_HUFFMAN,
    e_LZ77
};  // END ECompressor


class CompressorFactory
{
public:

    void compress(const std::string& inputFilePath, const std::string& outputFilePath, ECompressor comp);
    void decompress(const std::string& inputFilePath, const std::string& outputFilePath, ECompressor comp);

private:

    std::unique_ptr<ICompressor> _get_compressor(ECompressor comp);
};  // END CompressorFactory