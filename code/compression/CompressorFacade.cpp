#include "CompressorFacade.h"

#include "huffman/HuffmanCompressor.h"
#include "lz77/LZ77Compressor.h"


void CompressorFacade::compress(const std::string& inputFilePath, const std::string& outputFilePath, ECompressor type)
{
    auto c = _get_compressor(type);
    c->compress(inputFilePath, outputFilePath);
}

void CompressorFacade::decompress(const std::string& inputFilePath, const std::string& outputFilePath, ECompressor type)
{
    auto c = _get_compressor(type);
    c->decompress(inputFilePath, outputFilePath);
}

std::unique_ptr<ICompressor> CompressorFacade::_get_compressor(ECompressor type)
{
    std::unique_ptr<ICompressor> ret;

    switch (type)
    {
        case ECompressor::e_HUFFMAN:
        {
            ret = std::make_unique<HuffmanCompressor>();
            break;
        }
        case ECompressor::e_LZ77:
        {
            ret = std::make_unique<LZ77Compressor>();
            break;
        }
        default:
            throw std::invalid_argument("Unsupported compressor type"); // in case of ECompressor extension
    }

    return ret;
}