#include "Encoders.h"

#include "compression/huffman/HuffmanCompressor.h"
#include "compression/lz77/LZ77Compressor.h"

#include "transform/bwt/BWTransform.h"
#include "transform/mtf/MTFTransform.h"


void SequentialEncoder::add_to_sequence(EEncoderType type)
{
    _encodeSequence.push_back(type);
}

void SequentialEncoder::clear_sequence()
{
    _encodeSequence.clear();
}

void SequentialEncoder::encode(const std::string& inputFilePath, const std::string& outputFilePath)
{
    // TODO: create temp files
    for (EEncoderType type : _encodeSequence)
    {
        _set_current_encoder(type);
        _encode(inputFilePath, outputFilePath);
    }
}

void SequentialEncoder::decode(const std::string& inputFilePath, const std::string& outputFilePath)
{
    // TODO: create temp files
    for (EEncoderType type : _encodeSequence)
    {
        _set_current_encoder(type);
        _decode(inputFilePath, outputFilePath);
    }
}

void SequentialEncoder::_set_current_encoder(EEncoderType type)
{
    switch (type)
    {
        case EEncoderType::e_HUFFMAN:
        {
            _currentEncoder = std::make_unique<HuffmanCompressor>();
            break;
        }
        case EEncoderType::e_LZ77:
        {
            _currentEncoder = std::make_unique<LZ77Compressor>();
            break;
        }
        case EEncoderType::e_BWT:
        {
            _currentEncoder = std::make_unique<BWTransform>();
            break;
        }
        case EEncoderType::e_MTF:
        {
            _currentEncoder = std::make_unique<MTFTransform>();
            break;
        }
        default:
            throw std::invalid_argument("Unsupported encode type"); // in case of EEncoderType extension
    }
}

void SequentialEncoder::_encode(const std::string& inputFilePath, const std::string& outputFilePath)
{
    _ASSERT(_currentEncoder != nullptr, "No encoder selected!");

    _currentEncoder->encode(inputFilePath, outputFilePath);
}

void SequentialEncoder::_decode(const std::string& inputFilePath, const std::string& outputFilePath)
{
    _ASSERT(_currentEncoder != nullptr, "No encoder selected!");

    _currentEncoder->decode(inputFilePath, outputFilePath);
}