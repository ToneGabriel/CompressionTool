#pragma once

#include <cstring>
#include <string>
#include <map>
#include <unordered_map>

#include "ICompressor.h"
#include "_HuffmanTree.h"


class HuffmanCompressor// : public ICompressor
{
private:
    using _Frequency_Map    = std::map<symbol_t, size_t>;                   // used ordered map to ensure the tree is built the same
    using _Code_Map         = std::unordered_map<symbol_t, std::string>;

private:

    symbol_t        _extension[EXTENSION_SIZE + 1];  // add null chr
    symbol_t        _padding = 0;

    std::string     _originalFilename;
    std::string     _compressedFilename;

public:

    HuffmanCompressor() = default;
    
    ~HuffmanCompressor() = default;

public:

    void compress(const std::string& filename);// override;

    void decompress(const std::string& filename);// override;

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

    // void _set_extension(const std::string& filename)
    // {
    //     ::strncpy(_extension, fs::path(filename).extension().string().c_str(), EXTENSION_SIZE);
    // }

    // std::string _replace_extension(const std::string& filename, const std::string& extension)
    // {
    //     return fs::path(filename).replace_extension(extension).string();
    // }

    size_t _compute_file_size(const std::string& filename) const;
};  // END HuffmanCompressor