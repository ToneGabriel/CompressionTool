#pragma once

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

    std::string     _originalFilename;
    std::string     _compressedFilename;
    size_t          _originalSize;
    size_t          _compressedSize;

public:

    HuffmanCompressor() = default;
    
    ~HuffmanCompressor();

public:

    void compress(const std::string& filename) override;

    void decompress(const std::string& filename) override;

    std::string get_original_file_name() const;

    std::string get_compressed_file_name() const;

    size_t get_original_file_size() const;      // in bytes

    size_t get_compressed_file_size() const;    // in bytes

    double get_compression_ratio() const;

private:

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

    void _write_frequency_map(std::ofstream& ofile);

    void _read_frequency_map(std::ifstream& ifile);

    void _encode_and_write_file(std::ifstream& ifile, std::ofstream& ofile);

    void _decode_and_write_file(std::ifstream& ifile, std::ofstream& ofile);
};  // END HuffmanCompressor