#include "EncoderFacade.h"

#include "compression/huffman/HuffmanCompressor.h"
#include "compression/lz77/LZ77Compressor.h"

#include "transform/bwt/BWTTransform.h"
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
    std::unique_ptr<IEncoder> ret;

    switch (type)
    {
        case EEncoder::e_HUFFMAN:
        {
            ret = std::make_unique<HuffmanCompressor>();
            break;
        }
        case EEncoder::e_LZ77:
        {
            ret = std::make_unique<LZ77Compressor>();
            break;
        }
        case EEncoder::e_BWT:
        {
            ret = std::make_unique<BWTTransform>();
            break;
        }
        case EEncoder::e_MTF:
        {
            ret = std::make_unique<MTFTransform>();
            break;
        }
        default:
            throw std::invalid_argument("Unsupported compressor type"); // in case of EEncoder extension
    }

    return ret;
}