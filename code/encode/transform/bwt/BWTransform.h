#pragma once

#include "IEncoder.h"


class BWTransform : public IEncoder    // Burrows-Wheeler Transform
{
public:

    BWTransform() = default;
    ~BWTransform() = default;

private:

    void _Encode(const std::string& inputFilePath, const std::string& outputFilePath) override;
    void _Decode(const std::string& inputFilePath, const std::string& outputFilePath) override;
};  // END BWTransform