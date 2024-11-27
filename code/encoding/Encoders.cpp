#include "Encoders.h"

#include "compression/huffman/HuffmanCompressor.h"
#include "compression/lz77/LZ77Compressor.h"

#include "transform/bwt/BWTransform.h"
#include "transform/mtf/MTFTransform.h"

#include "../helpers/TemporaryFileWorkspace.h"


// BasicEncoder =====================================================
void BasicEncoder::set_type(EEncoderType type)
{
    _currentEncoderType = type;
}

EEncoderType BasicEncoder::get_type() const
{
    return _currentEncoderType;
}

void BasicEncoder::encode(const std::string& inputFilePath, const std::string& outputFilePath)
{
    _get_encoder(_currentEncoderType)->encode(inputFilePath, outputFilePath);
}

void BasicEncoder::decode(const std::string& inputFilePath, const std::string& outputFilePath)
{
    _get_encoder(_currentEncoderType)->decode(inputFilePath, outputFilePath);
}

std::unique_ptr<IEncoder> BasicEncoder::_get_encoder(EEncoderType type)
{
    switch (type)
    {
        case EEncoderType::e_HUFFMAN:
        {
            return std::make_unique<HuffmanCompressor>();
        }
        case EEncoderType::e_LZ77:
        {
            return std::make_unique<LZ77Compressor>();
        }
        case EEncoderType::e_BWT:
        {
            return std::make_unique<BWTransform>();
        }
        case EEncoderType::e_MTF:
        {
            return std::make_unique<MTFTransform>();
        }
        default:
            throw std::invalid_argument("Unsupported encoder type"); // in case of EEncoderType extension
    }
}


// SequentialEncoder =====================================================
void SequentialEncoder::add_to_sequence(EEncoderType type)
{
    _encodeSequence.push_back(type);
}

void SequentialEncoder::clear_sequence()
{
    _encodeSequence.clear();
}

bool SequentialEncoder::empty_sequence() const
{
    return _encodeSequence.empty();
}

void SequentialEncoder::encode(const std::string& inputFilePath, const std::string& outputFilePath)
{
    _ASSERT(!empty_sequence(), "Empty sequence!");

    try
    {
        // Create a dedicated working folder in the temporary directory (gets deleted automatically)
        TemporaryFileWorkspace tempWorkspace(2);

        // Encoder to do the work
        BasicEncoder encoder;

        std::string currentInputFile = inputFilePath;
        std::string currentOutputFile;

        for (EEncoderType type : _encodeSequence)
        {
            // Generate a temporary output file path
            currentOutputFile = tempWorkspace.create_local_file();

            // Encode
            encoder.set_type(type);
            encoder.encode(currentInputFile, currentOutputFile);

            // Update the input file for the next step
            currentInputFile = currentOutputFile;
        }

        // Move the last encoded file to the final name
        tempWorkspace.move_local_file(currentInputFile, outputFilePath);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        RERAISE;
    }
}

void SequentialEncoder::decode(const std::string& inputFilePath, const std::string& outputFilePath)
{
    _ASSERT(!empty_sequence(), "Empty sequence!");

    try
    {
        // Create a dedicated working folder in the temporary directory (gets deleted automatically)
        TemporaryFileWorkspace tempWorkspace(2);

        // Encoder to do the work
        BasicEncoder encoder;

        std::string currentInputFile = inputFilePath;
        std::string currentOutputFile;

        for (EEncoderType type : _encodeSequence)
        {
            // Generate a temporary output file path
            currentOutputFile = tempWorkspace.create_local_file();

            // Decode
            encoder.set_type(type);
            encoder.decode(currentInputFile, currentOutputFile);

            // Update the input file for the next step
            currentInputFile = currentOutputFile;
        }

        // Move the last encoded file to the final name
        tempWorkspace.move_local_file(currentInputFile, outputFilePath);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        RERAISE;
    }
}