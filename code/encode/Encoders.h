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


class SequentialEncoder
{
private:
    std::vector<EEncoderType> _encoderTypeSequence;

public:

    SequentialEncoder() = default;
    ~SequentialEncoder() = default;

    SequentialEncoder(const SequentialEncoder&) = default;
    SequentialEncoder& operator=(const SequentialEncoder&) = default;

private:

    static std::unique_ptr<IEncoder> _GetEncoder(EEncoderType type);   // factory method

public:

    void AddToSequence(EEncoderType type);
    void ClearSequence();
    bool EmptySequence() const;

    void Process(   EEncodingDirection direction,
                    const std::string& inputFilePath,
                    const std::string& outputFilePath);
};   // END SequentialEncoder