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
    e_MTF,

    e_DEFAULT = e_HUFFMAN
};  // END EEncoderType


class BasicEncoder  // Facade for using different encoders and controlling their lifecycle
{
private:
    EEncoderType _currentEncoderType;

public:

    BasicEncoder(EEncoderType type = EEncoderType::e_DEFAULT)
        : _currentEncoderType(type) { /*Empty */ }

    ~BasicEncoder() = default;

    BasicEncoder(const BasicEncoder&) = default;
    BasicEncoder& operator=(const BasicEncoder&) = default;

public:

    void set_type(EEncoderType type);
    EEncoderType get_type() const;

    void encode(const std::string& inputFilePath, const std::string& outputFilePath);
    void decode(const std::string& inputFilePath, const std::string& outputFilePath);

private:
    static std::unique_ptr<IEncoder> _get_encoder(EEncoderType type);   // factory method
};  // END BasicEncoder


class SequentialEncoder
{
private:
    std::vector<EEncoderType> _encodeSequence;

public:

    SequentialEncoder()     = default;
    ~SequentialEncoder()    = default;

    SequentialEncoder(const SequentialEncoder&)             = default;
    SequentialEncoder& operator=(const SequentialEncoder&)  = default;

public:

    void add_to_sequence(EEncoderType type);
    void clear_sequence();
    bool empty_sequence() const;

    void encode(const std::string& inputFilePath, const std::string& outputFilePath);
    void decode(const std::string& inputFilePath, const std::string& outputFilePath);

private:

    std::string _get_encode_file_path(const std::string& path) const;
    std::string _get_decode_file_path(const std::string& path) const;
};   // END SequentialEncoder