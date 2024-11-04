#pragma once

#include <list>

#include "IEncoder.h"


class MTFTransform : public IEncoder    // Move-to-front encoding
{
public:

    void encode(const std::string& inputFilePath, const std::string& outputFilePath) override;
    void decode(const std::string& inputFilePath, const std::string& outputFilePath) override;

private:

    std::list<char> _create_alphabet_list() const;
};  // END MTFTransform