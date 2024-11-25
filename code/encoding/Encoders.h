#pragma once

#include <string>
#include <memory>
#include <vector>

#include "_Core.h"
#include "IEncoder.h"


enum class EEncoderType
{
    e_HUFFMAN,
    e_LZ77,
    e_BWT,
    e_MTF
};  // END EEncoderType


class SequentialEncoder
{
private:
    std::vector<EEncoderType>   _encodeSequence;
    std::unique_ptr<IEncoder>   _currentEncoder;
    std::string                 _workingDirectory;

public:

    SequentialEncoder()     = default;
    ~SequentialEncoder()    = default;

    SequentialEncoder(const SequentialEncoder&)             = default;
    SequentialEncoder& operator=(const SequentialEncoder&)  = default;

public:

    void add_to_sequence(EEncoderType type);
    void clear_sequence();
    bool empty_sequence() const;

    void encode(const std::string& inputFilePath);
    void decode(const std::string& inputFilePath);

private:

    void _clear_current_encoder();
    void _set_current_encoder(EEncoderType type);   // factory method

    void _encode_current(const std::string& inputFilePath, const std::string& outputFilePath);
    void _decode_current(const std::string& inputFilePath, const std::string& outputFilePath);

    std::string _get_encode_file_path(const std::string& path) const;
    std::string _get_decode_file_path(const std::string& path) const;

    void _create_working_directory();
    void _remove_working_directory();

    std::string _create_temp_file_name(size_t index) const;
};   // END SequentialEncoder