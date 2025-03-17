#include "Encoders.h"

#include "compress/huffman/HuffmanCompressor.h"
#include "compress/lz77/LZ77Compressor.h"

#include "transform/bwt/BWTransform.h"
#include "transform/mtf/MTFTransform.h"

#include "../helpers/TemporaryFileWorkspace.h"

// SequentialEncoder =====================================================
std::unique_ptr<IEncoder> SequentialEncoder::_GetEncoder(EEncoderType type)
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

void SequentialEncoder::AddToSequence(EEncoderType type)
{
    _encoderTypeSequence.push_back(type);
}

void SequentialEncoder::ClearSequence()
{
    _encoderTypeSequence.clear();
}

bool SequentialEncoder::EmptySequence() const
{
    return _encoderTypeSequence.empty();
}

void SequentialEncoder::Process(EEncodingDirection direction,
                                const std::string& inputFilePath,
                                const std::string& outputFilePath)
{
    _ASSERT(!EmptySequence(), "Empty sequence!");

    try
    {
        // Create a dedicated working folder in the temporary directory (gets deleted automatically)
        TemporaryFileWorkspace tempWorkspace(2);

        std::string currentInputFile = inputFilePath;
        std::string currentOutputFile;

        for (EEncoderType type : _encoderTypeSequence)
        {
            // Generate a temporary output file path
            currentOutputFile = tempWorkspace.create_local_file();

            // Encode
            _GetEncoder(type)->Process(direction, currentInputFile, currentOutputFile);

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
