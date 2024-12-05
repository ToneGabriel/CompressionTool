#pragma once

#include "IEncoder.h"

#include <list>


class MTFTransform : public IEncoder    // Move-to-front encoding
{
public:

    MTFTransform() = default;
    ~MTFTransform() = default;

public:

    void encode(const std::string& inputFilePath, const std::string& outputFilePath) override;
    void decode(const std::string& inputFilePath, const std::string& outputFilePath) override;

private:

    static std::list<char> _create_alphabet_list();
};  // END MTFTransform