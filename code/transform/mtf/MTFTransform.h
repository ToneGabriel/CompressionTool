#pragma once

#include "IEncoder.h"


class MTFTransform : public IEncoder    // Move-to-front encoding
{

public:

    void encode(const std::string& inputFilePath, const std::string& outputFilePath) override;
    void decode(const std::string& inputFilePath, const std::string& outputFilePath) override;
};  // END MTFTransform