#pragma once

#include <string>
#include <bitset>
#include <queue>
#include <map>
#include <unordered_map>

#include <iostream>
#include <fstream>
#include <filesystem>

#include "_Core.h"

#define HUFFMAN_BEGIN namespace huffman {
#define HUFFMAN_END }

namespace fs = std::filesystem;

struct _Huffman_Node
{
    symbol_t        _Symbol;
    size_t          _Frequency;
    _Huffman_Node*  _Left;
    _Huffman_Node*  _Right;

    _Huffman_Node() = delete;
    ~_Huffman_Node() = default;

    _Huffman_Node(symbol_t symbol, size_t frequency, _Huffman_Node* left = nullptr, _Huffman_Node* right = nullptr);

    _Huffman_Node(const _Huffman_Node& other) = delete;
    _Huffman_Node& operator=(const _Huffman_Node& other) = delete;
};  // END _Huffman_Node


struct _Huffman_NodePtr_Greater_Compare
{
    bool operator()(const _Huffman_Node* const left, const _Huffman_Node* const right);
};  // END _Huffman_NodePtr_Greater_Compare


class Huffman_Compressor
{
private:
    using _Frequency_Map    = std::map<symbol_t, size_t>;                   // used ordered map to ensure the tree is built the same
    using _Code_Map         = std::unordered_map<symbol_t, std::string>;

private:
    _Frequency_Map  _frequencyMap;
    _Code_Map       _codeMap;
    _Huffman_Node*  _root = nullptr;

public:

    Huffman_Compressor() = default;

    ~Huffman_Compressor()
    {
        _delete_tree();
    }

public:

    void print_tree()
    {
        _print_tree();
    }

    void compress(const std::string& filename)
    {
        _ASSERT(fs::exists(filename), "File not found!");

        std::ifstream ifile(filename, std::ios::binary);
        std::ofstream ofile(fs::path(filename).replace_extension(BIN_EXTENSION).string(), std::ios::binary);

        _compute_symbol_frequencies(ifile);
        _build_tree();
        _generate_huffman_codes();
        _write_frequency_map(ofile);
        _encode_and_write_file(ifile, ofile);

        ifile.close();
        ofile.close();
    }

    void decompress(const std::string& filename)
    {
        _ASSERT(fs::exists(filename), "File not found!");

        std::ifstream ifile(filename, std::ios::binary);
        std::ofstream ofile(fs::path(filename).replace_extension(".txtd").string(), std::ios::binary);

        _read_frequency_map(ifile);
        _build_tree();
        _decode_and_write_file(ifile, ofile);

        ifile.close();
        ofile.close();
    }

private:

    void _build_tree()
    {
        _ASSERT(!_frequencyMap.empty(), "No frequency data!");

        // create leaf nodes for each symbol and push them to the priority queue
        std::priority_queue<_Huffman_Node*,
                            std::vector<_Huffman_Node*>,
                            _Huffman_NodePtr_Greater_Compare> prioq;

        _Huffman_Node* leftNode     = nullptr;
        _Huffman_Node* rightNode    = nullptr;

        for (const auto& [symbol, frequency] : _frequencyMap)
            prioq.push(new _Huffman_Node(symbol, frequency));

        while (prioq.size() > 1)    // Last elem will be root
        {
            // remove two nodes of lowest frequency
            leftNode = prioq.top();
            prioq.pop();

            rightNode = prioq.top();
            prioq.pop();
            
            // create a new internal subroot with these two nodes as children
            prioq.push(new _Huffman_Node(   static_cast<symbol_t>(0),
                                            leftNode->_Frequency + rightNode->_Frequency,
                                            leftNode,
                                            rightNode));
        }

        // last elem is root
        _root = prioq.top();
        prioq.pop();
    }

    void _delete_tree_impl(_Huffman_Node* subroot)
    {
        if (subroot == nullptr)
            return;

        _delete_tree_impl(subroot->_Left);
        _delete_tree_impl(subroot->_Right);

        delete subroot;
    }

    void _delete_tree()
    {
        _delete_tree_impl(_root);
    }

    void _print_tree_impl(const size_t ident, const _Huffman_Node* const subroot, const std::string& property) const
    {
        if (subroot != nullptr)
            std::cout   << std::string("").append(ident, '\t')
                        << subroot->_Symbol
                        << " [" << subroot->_Frequency << " "<< property << "]\n";

        if (subroot->_Left != nullptr)
            _print_tree_impl(ident + 1, subroot->_Left, "LEFT");

        if (subroot->_Right != nullptr)
            _print_tree_impl(ident + 1, subroot->_Right, "RIGHT");
    }

    void _print_tree() const
    {
        _print_tree_impl(0, _root, "ROOT");
    }

    void _compute_symbol_frequencies(std::ifstream& ifile)
    {
        symbol_t symbol;

        while (ifile.get(symbol))
            ++_frequencyMap[symbol];

        ifile.clear();              // clear error flags
        ifile.seekg(std::ios::beg); // reset file to beginning
    }

    void _generate_huffman_codes_impl(  const _Huffman_Node* const subroot,
                                        const std::string& code,
                                        std::unordered_map<symbol_t, std::string>& codes)
    {
        if (subroot == nullptr)
            return;

        // if it's a leaf node, store the code
        if (subroot->_Left == nullptr && subroot->_Right == nullptr)
        {
            codes[subroot->_Symbol] = code;
            return;
        }

        // traverse left subtree, append '0'
        _generate_huffman_codes_impl(subroot->_Left, code + ZERO_CHR, codes);

        // traverse right subtree, append '1'
        _generate_huffman_codes_impl(subroot->_Right, code + ONE_CHR, codes);
    }

    void _generate_huffman_codes()
    {
        _generate_huffman_codes_impl(_root, "", _codeMap);
    }

    void _write_frequency_map(std::ofstream& ofile)
    {
        _ASSERT(ofile.is_open(), "Output file is not open!");

        size_t mapSize = _frequencyMap.size();
        ofile.write(reinterpret_cast<const char*>(&mapSize), sizeof(mapSize));

        for (const auto& [symbol, count] : _frequencyMap)
        {
            ofile.write(reinterpret_cast<const char*>(&symbol), sizeof(symbol));
            ofile.write(reinterpret_cast<const char*>(&count), sizeof(count));
        }

        // DON'T close file
    }

    void _read_frequency_map(std::ifstream& ifile)
    {
        _ASSERT(ifile.is_open(), "Input file is not open!");

        size_t      mapSize;
        symbol_t    symbol;
        size_t      count;

        ifile.read(reinterpret_cast<char*>(&mapSize), sizeof(mapSize));

        for (size_t i = 0; i < mapSize; ++i)
        {

            ifile.read(reinterpret_cast<char*>(&symbol), sizeof(symbol));
            ifile.read(reinterpret_cast<char*>(&count), sizeof(count));

            _frequencyMap[symbol] = count;
        }
    }

    void _encode_and_write_file(std::ifstream& ifile, std::ofstream& ofile)
    {
        _ASSERT(ifile.is_open(), "Input file is not open!");
        _ASSERT(ofile.is_open(), "Output file is not open!");

        std::string buffer;
        symbol_t    symbol;

        while (ifile.get(symbol))
        {
            buffer.append(_codeMap.at(symbol));

            while (buffer.size() >= SYMBOL_BIT)
            {
                // take necessary bits from buffer and transform them into bitset
                std::bitset<SYMBOL_BIT> bits(buffer, 0, SYMBOL_BIT);

                // remaining bits will be used in next iterations
                buffer.erase(0, SYMBOL_BIT);

                // transform bits from bitset into a symbol and write it
                ofile.put(static_cast<symbol_t>(bits.to_ulong()));
            }
        }

        // pad leftover bits in the buffer with 0s and write them as the last byte
        if (!buffer.empty())
        {
            // Pad the remaining bits to form a full byte
            buffer.append(SYMBOL_BIT - buffer.size(), ZERO_CHR);
            std::bitset<SYMBOL_BIT> bits(buffer);

            ofile.put(static_cast<symbol_t>(bits.to_ulong()));
        }

        // DON'T close files
    }

    void _decode_and_write_file(std::ifstream& ifile, std::ofstream& ofile)
    {
        _ASSERT(ifile.is_open(), "Input file is not open!");
        _ASSERT(ofile.is_open(), "Output file is not open!");

        _Huffman_Node*  currentNode = _root;
        std::string     buffer;
        symbol_t        symbol;

        while (ifile.get(symbol))
        {
            std::bitset<SYMBOL_BIT> bits(symbol);
            buffer = std::move(bits.to_string());

            for (symbol_t digit : buffer)
            {
                if (digit == ZERO_CHR)
                    currentNode = currentNode->_Left;
                else
                    currentNode = currentNode->_Right;

                if (currentNode->_Left == nullptr && currentNode->_Right == nullptr)
                {
                    ofile.put(currentNode->_Symbol);    // write the character to the output file
                    currentNode = _root;                // reset to root for next character
                }
                else
                {
                    // do nothing - currentNode remains the same
                }
            }
        }
    }
};  // END Huffman_Compressor