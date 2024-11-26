#pragma once

#include "IEncoder.h"
#include "_HuffmanTree.h"

#include <string>
#include <map>
#include <unordered_map>


class HuffmanCompressor : public IEncoder
{
private:
    using _Frequency_Map    = std::map<symbol_t, size_t>;                   // used ordered map to ensure the tree is built the same
    using _Code_Map         = std::unordered_map<symbol_t, std::string>;

private:

    symbol_t        _extension[10 + 1];  // add null chr
    symbol_t        _padding = 0;

    std::string     _originalFilename;
    std::string     _compressedFilename;

public:

    HuffmanCompressor() = default;
    ~HuffmanCompressor() = default;

public:

    void encode(const std::string& inputFilePath, const std::string& outputFilePath) override;
    void decode(const std::string& inputFilePath, const std::string& outputFilePath) override;

private:

    void _compute_padding(  const std::map<symbol_t, size_t>& frequencyMap,
                            const std::unordered_map<symbol_t, std::string>& codeMap)
    {
        size_t totalCompressedBits  = 0;
        symbol_t leftoverBits       = 0;

        for (auto& [symbol, count] : frequencyMap)
            totalCompressedBits += (count * codeMap.at(symbol).size());

        leftoverBits = totalCompressedBits % SYMBOL_BIT;

        if (leftoverBits == 0)
            _padding = 0;
        else
            _padding = SYMBOL_BIT - leftoverBits;
    }
};  // END HuffmanCompressor