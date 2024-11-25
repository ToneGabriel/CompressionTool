#include "Encoders.h"

#include "compression/huffman/HuffmanCompressor.h"
#include "compression/lz77/LZ77Compressor.h"

#include "transform/bwt/BWTransform.h"
#include "transform/mtf/MTFTransform.h"

#include <filesystem>

namespace fs = std::filesystem;


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

void SequentialEncoder::encode(const std::string& inputFilePath)
{
    _ASSERT(!empty_sequence(), "Empty sequence!");

    // Create a dedicated working folder in the temporary directory
    _create_working_directory();

    std::string currentInputFile = inputFilePath;
    std::string currentOutputFile;

    for (size_t i = 0; i < _encodeSequence.size(); ++i)
    {
        // Set the current encoder
        _set_current_encoder(_encodeSequence[i]);

        // Generate a temporary output file path
        currentOutputFile = _create_temp_file_name(i);

        // Encode
        _encode_current(currentInputFile, currentOutputFile);

        // Update the input file for the next step
        currentInputFile = currentOutputFile;
    }

    // Move the last encoded file to the final name
    fs::rename(currentInputFile, _get_encode_file_path(inputFilePath));

    // Clean up encoder
    _clear_current_encoder();

    // Clean up temporary working directory
    _remove_working_directory();
}

void SequentialEncoder::decode(const std::string& inputFilePath)
{
    _ASSERT(!empty_sequence(), "Empty sequence!");

    // Create a dedicated working folder in the temporary directory
    _create_working_directory();

    std::string currentInputFile = inputFilePath;
    std::string currentOutputFile;

    for (size_t i = 0; i < _encodeSequence.size(); ++i)
    {
        // Set the current encoder
        _set_current_encoder(_encodeSequence[i]);

        // Generate a temporary output file path
        currentOutputFile = _create_temp_file_name(i);

        // Decode
        _decode_current(currentInputFile, currentOutputFile);

        // Update the input file for the next step
        currentInputFile = currentOutputFile;
    }

    // Move the last encoded file to the final name
    fs::rename(currentInputFile, _get_decode_file_path(inputFilePath));

    // Clean up encoder
    _clear_current_encoder();

    // Clean up temporary working directory
    _remove_working_directory();
}

void SequentialEncoder::_clear_current_encoder()
{
    (void)_currentEncoder.release();
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
            throw std::invalid_argument("Unsupported encoder type"); // in case of EEncoderType extension
    }
}

void SequentialEncoder::_encode_current(const std::string& inputFilePath, const std::string& outputFilePath)
{
    _ASSERT(_currentEncoder.get() != nullptr, "No encoder selected!");

    _currentEncoder->encode(inputFilePath, outputFilePath);
}

void SequentialEncoder::_decode_current(const std::string& inputFilePath, const std::string& outputFilePath)
{
    _ASSERT(_currentEncoder.get() != nullptr, "No encoder selected!");

    _currentEncoder->decode(inputFilePath, outputFilePath);
}

std::string SequentialEncoder::_get_encode_file_path(const std::string& path) const
{
    std::string ret = path + BIN_EXTENSION;
    return ret;
}

std::string SequentialEncoder::_get_decode_file_path(const std::string& path) const
{
    std::string ret = path;
    ret.erase(path.size() - sizeof(BIN_EXTENSION) + 1);
    return ret;
}

void SequentialEncoder::_create_working_directory()
{
    fs::path tempDir = fs::temp_directory_path() / fs::path("sequential_encoder_" + std::to_string(std::time(nullptr)));
    fs::create_directory(tempDir);
    _workingDirectory = tempDir.string();
}

void SequentialEncoder::_remove_working_directory()
{
    if (fs::exists(_workingDirectory))
        fs::remove_all(_workingDirectory); // Remove the working directory and its contents
}

std::string SequentialEncoder::_create_temp_file_name(size_t index) const
{
    fs::path tempFilePath = fs::path(_workingDirectory) / fs::path("temp_" + std::to_string(index) + BIN_EXTENSION);
    return tempFilePath.string();
}