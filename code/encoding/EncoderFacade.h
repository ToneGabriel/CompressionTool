#pragma once

#include <string>
#include <memory>

#include "IEncoder.h"


enum class EEncoder
{
    e_HUFFMAN,
    e_LZ77,
    e_BWT,
    e_MTF
};  // END EEncoder


class EncoderFacade
{
public:

    void encode(const std::string& inputFilePath, const std::string& outputFilePath, EEncoder type);
    void decode(const std::string& inputFilePath, const std::string& outputFilePath, EEncoder type);

private:

    std::unique_ptr<IEncoder> _get_encoder(EEncoder type); // factory method
};  // END EncoderFacade