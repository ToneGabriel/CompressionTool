#include "CompressorFactory.h"


void CompressorFactory::compress(const std::string& inputFilePath, const std::string& outputFilePath, ECompressor comp)
{
    auto c = _get_compressor(comp);
    c->compress(inputFilePath, outputFilePath);
}

void CompressorFactory::decompress(const std::string& inputFilePath, const std::string& outputFilePath, ECompressor comp)
{
    auto c = _get_compressor(comp);
    c->decompress(inputFilePath, outputFilePath);
}

std::unique_ptr<ICompressor> CompressorFactory::_get_compressor(ECompressor comp) // factory method
{
    std::unique_ptr<ICompressor> ret;

    switch (comp)
    {
        case ECompressor::e_HUFFMAN:
        {
            // ret = std::make_unique<HuffmanCompressor>();
            break;
        }
        case ECompressor::e_LZ77:
        {
            // ret = std::make_unique<LZ77Compressor>();
            break;
        }
        default:
            break;
    }

    return ret;
}