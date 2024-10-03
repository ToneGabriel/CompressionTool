#pragma once

#include <cstring>
#include <string>
#include <bitset>
#include <queue>
#include <map>
#include <unordered_map>

#include <iostream>
#include <fstream>
#include <filesystem>

#include "ICompressor.h"
#include "_HuffmanNode.h"


namespace fs = std::filesystem;


class HuffmanCompressor : public ICompressor
{
private:
    using _Frequency_Map    = std::map<symbol_t, size_t>;                   // used ordered map to ensure the tree is built the same
    using _Code_Map         = std::unordered_map<symbol_t, std::string>;

private:
    _Frequency_Map  _frequencyMap;
    _Code_Map       _codeMap;
    _HuffmanNode*   _root = nullptr;

    symbol_t        _extension[EXTENSION_SIZE + 1];  // add null chr
    symbol_t        _padding = 0;

    std::string     _originalFilename;
    std::string     _compressedFilename;

public:

    HuffmanCompressor() = default;
    
    ~HuffmanCompressor();

public:

    void compress(const std::string& filename) override;

    void decompress(const std::string& filename) override;

private:

    void _compute_padding()
    {
        size_t totalCompressedBits  = 0;
        symbol_t leftoverBits       = 0;

        for (auto& [symbol, count] : _frequencyMap)
            totalCompressedBits += (count * _codeMap[symbol].size());

        leftoverBits = totalCompressedBits % SYMBOL_BIT;

        if (leftoverBits == 0)
            _padding = 0;
        else
            _padding = SYMBOL_BIT - leftoverBits;
    }

    void _set_extension(const std::string& filename)
    {
        ::strncpy(_extension, fs::path(filename).extension().string().c_str(), EXTENSION_SIZE);
    }

    std::string _replace_extension(const std::string& filename, const std::string& extension)
    {
        return fs::path(filename).replace_extension(extension).string();
    }

    size_t _compute_file_size(const std::string& filename) const;

    void _build_tree();

    void _delete_tree_impl(_HuffmanNode* subroot);

    void _delete_tree();

    void _print_tree_impl(const size_t ident, const _HuffmanNode* const subroot, const std::string& property) const;

    void _print_tree() const;

    void _compute_symbol_frequencies(std::ifstream& ifile);

    void _generate_huffman_codes_impl(  const _HuffmanNode* const subroot,
                                        const std::string& code,
                                        std::unordered_map<symbol_t, std::string>& codes);

    void _generate_huffman_codes();

    void _write_metadata(std::ofstream& ofile);

    void _read_metadata(std::ifstream& ifile);

    void _encode_and_write_file(std::ifstream& ifile, std::ofstream& ofile);

    void _decode_and_write_file(std::ifstream& ifile, std::ofstream& ofile);
};  // END HuffmanCompressor