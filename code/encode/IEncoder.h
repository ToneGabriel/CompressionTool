#pragma once

#include <string>


class IEncoder
{
public:

    virtual void encode(const std::string& inputFilePath, const std::string& outputFilePath) = 0;
    virtual void decode(const std::string& inputFilePath, const std::string& outputFilePath) = 0;

    virtual ~IEncoder() = default;
};  // END IEncoder