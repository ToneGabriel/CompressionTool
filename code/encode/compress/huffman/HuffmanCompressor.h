#pragma once

#include "IEncoder.h"
#include "_HuffmanTree.h"

#include <string>
#include <map>
#include <unordered_map>


class HuffmanCompressor : public IEncoder
{
public:

    HuffmanCompressor() = default;
    ~HuffmanCompressor() = default;

private:

    void _Encode(const std::string& inputFilePath, const std::string& outputFilePath) override;
    void _Decode(const std::string& inputFilePath, const std::string& outputFilePath) override;

    static size_t _compute_padding( const std::map<symbol_t, size_t>& frequencyMap,
                                    const std::unordered_map<symbol_t, std::string>& codeMap);
};  // END HuffmanCompressor