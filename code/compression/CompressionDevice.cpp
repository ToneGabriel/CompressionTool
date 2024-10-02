#include "CompressionDevice.h"


void CompressionDevice::compress(const std::string& path, ECompressor comp)
{
    auto c = _get_compressor(comp);
    c->compress(path);
}

void CompressionDevice::decompress(const std::string& path, ECompressor comp)
{
    auto c = _get_compressor(comp);
    c->decompress(path);
}

std::unique_ptr<ICompressor> CompressionDevice::_get_compressor(ECompressor comp) // factory method
{
    std::unique_ptr<ICompressor> ret;

    switch (comp)
    {
        case ECompressor::e_HUFFMAN:
        {
            ret = std::make_unique<HuffmanCompressor>();
            break;
        }
        case ECompressor::e_LZ77:
        {
            // TODO
            break;
        }
        default:
            break;
    }

    return ret;
}