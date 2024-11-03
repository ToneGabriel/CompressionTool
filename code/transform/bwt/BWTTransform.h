#pragma once

#include "IEncoder.h"


class BWTTransform : public IEncoder    // Burrows-Wheeler Transform
{

public:

    void encode(const std::string& inputFilePath, const std::string& outputFilePath) override;
    void decode(const std::string& inputFilePath, const std::string& outputFilePath) override;
};  // END BWTTransform