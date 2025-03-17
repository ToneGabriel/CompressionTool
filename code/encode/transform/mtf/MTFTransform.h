#pragma once

#include "IEncoder.h"

#include <list>


class MTFTransform : public IEncoder    // Move-to-front encoding
{
public:

    MTFTransform() = default;
    ~MTFTransform() = default;

private:

    void _Encode(const std::string& inputFilePath, const std::string& outputFilePath) override;
    void _Decode(const std::string& inputFilePath, const std::string& outputFilePath) override;

    static std::list<char> _create_alphabet_list();
};  // END MTFTransform