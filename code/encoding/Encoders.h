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
    std::vector<EEncoderType> _encodeSequence;
    std::unique_ptr<IEncoder> _currentEncoder;

public:

    SequentialEncoder()     = default;
    ~SequentialEncoder()    = default;

    SequentialEncoder(const SequentialEncoder&)             = default;
    SequentialEncoder& operator=(const SequentialEncoder&)  = default;

public:

    void add_to_sequence(EEncoderType method);
    void clear_sequence();

    void encode(const std::string& inputFilePath, const std::string& outputFilePath);
    void decode(const std::string& inputFilePath, const std::string& outputFilePath);

private:

    void _set_current_encoder(EEncoderType method);   // factory method

    void _encode(const std::string& inputFilePath, const std::string& outputFilePath);
    void _decode(const std::string& inputFilePath, const std::string& outputFilePath);
};   // END SequentialEncoder