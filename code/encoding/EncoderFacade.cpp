#include "EncoderFacade.h"

#include "compression/huffman/HuffmanCompressor.h"
#include "compression/lz77/LZ77Compressor.h"

#include "transform/bwt/BWTransform.h"
#include "transform/mtf/MTFTransform.h"


void EncoderFacade::encode(const std::string& inputFilePath, const std::string& outputFilePath, EEncoder type)
{
    auto c = _get_encoder(type);
    c->encode(inputFilePath, outputFilePath);
}

void EncoderFacade::decode(const std::string& inputFilePath, const std::string& outputFilePath, EEncoder type)
{
    auto c = _get_encoder(type);
    c->decode(inputFilePath, outputFilePath);
}

std::unique_ptr<IEncoder> EncoderFacade::_get_encoder(EEncoder type)
{
    switch (type)
    {
        case EEncoder::e_HUFFMAN:
        {
            return std::make_unique<HuffmanCompressor>();
        }
        case EEncoder::e_LZ77:
        {
            return std::make_unique<LZ77Compressor>();
        }
        case EEncoder::e_BWT:
        {
            return std::make_unique<BWTransform>();
        }
        case EEncoder::e_MTF:
        {
            return std::make_unique<MTFTransform>();
        }
        default:
            throw std::invalid_argument("Unsupported encoder type"); // in case of EEncoder extension
    }
}