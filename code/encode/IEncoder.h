#pragma once

#include <string>

enum class EEncodingDirection
{
    e_FORWARD,
    e_BACKWARD
};  // END EEncodingDirection


class IEncoder
{
public:

    virtual ~IEncoder() = default;

public:

    void Process(   EEncodingDirection direction,
                    const std::string& inputFilePath,
                    const std::string& outputFilePath)
    {
        switch (direction)
        {
            case EEncodingDirection::e_FORWARD:
            {
                _Encode(inputFilePath, outputFilePath);
                return;
            }
            case EEncodingDirection::e_BACKWARD:
            {
                _Decode(inputFilePath, outputFilePath);
                return;
            }
            default:
                return;
        }
    }   

private:

    virtual void _Encode(const std::string& inputFilePath, const std::string& outputFilePath) = 0;
    virtual void _Decode(const std::string& inputFilePath, const std::string& outputFilePath) = 0;
};  // END IEncoder
