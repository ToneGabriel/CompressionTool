#include "HuffmanCompressor.h"

#include <bitset>
#include <queue>

#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

void HuffmanCompressor::compress(const std::string& inputFilePath, const std::string& outputFilePath)
{
    _ASSERT(fs::exists(inputFilePath), "File not found!");

    _HuffmanTree                                tree;
    std::map<symbol_t, size_t>                  frequencyMap;
    std::unordered_map<symbol_t, std::string>   codeMap;

    _originalFilename   = inputFilePath;
    _compressedFilename = fs::path(inputFilePath).replace_extension(HUFFMAN_EXTENSION).string();

    std::ifstream ifile(_originalFilename, std::ios::binary);
    std::ofstream ofile(_compressedFilename, std::ios::binary);

    {   // compute symbol frequencies
        symbol_t symbol;

        while (ifile.get(symbol))
            ++frequencyMap[symbol];

        ifile.clear();              // clear error flags
        ifile.seekg(std::ios::beg); // reset file to beginning
    }

    tree.build(frequencyMap);
    codeMap = tree.generate_huffman_codes();

    {   // write metadata

        // write extension
        ofile.write(reinterpret_cast<const char*>(&_extension), sizeof(_extension));

        // write padding
        ofile.write(reinterpret_cast<const char*>(&_padding), sizeof(_padding));

        // write frequency map and its size
        size_t mapSize = frequencyMap.size();
        ofile.write(reinterpret_cast<const char*>(&mapSize), sizeof(mapSize));

        for (const auto& [symbol, count] : frequencyMap)
        {
            ofile.write(reinterpret_cast<const char*>(&symbol), sizeof(symbol));
            ofile.write(reinterpret_cast<const char*>(&count), sizeof(count));
        }
    }

    {   // encode and write file
        std::string buffer;
        symbol_t    symbol;

        while (ifile.get(symbol))
        {
            buffer.append(codeMap.at(symbol));

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
        if (_padding != 0)
        {
            // Pad the remaining bits to form a full byte
            buffer.append(_padding, SYMBOL_ZERO);
            std::bitset<SYMBOL_BIT> bits(buffer);

            ofile.put(static_cast<symbol_t>(bits.to_ulong()));
        }
        else
        {
            // do nothing - no padding needed
        }
    }   // END encode and write file

    ifile.close();
    ofile.close();
}

void HuffmanCompressor::decompress(const std::string& inputFilePath, const std::string& outputFilePath)
{
    _ASSERT(fs::exists(inputFilePath), "File not found!");

    _HuffmanTree                                tree;
    std::map<symbol_t, size_t>                  frequencyMap;
    std::unordered_map<symbol_t, std::string>   codeMap;

    _originalFilename   = fs::path(inputFilePath).replace_extension(".txtd").string();
    _compressedFilename = inputFilePath;

    std::ifstream ifile(_compressedFilename, std::ios::binary);
    std::ofstream ofile(_originalFilename, std::ios::binary);

    {   // read metadata

        // read extension
        ifile.read(reinterpret_cast<char*>(&_extension), sizeof(_extension));

        // read padding
        ifile.read(reinterpret_cast<char*>(&_padding), sizeof(_padding));

        // read frequency map and its size
        size_t      mapSize;
        symbol_t    symbol;
        size_t      count;

        ifile.read(reinterpret_cast<char*>(&mapSize), sizeof(mapSize));

        for (size_t i = 0; i < mapSize; ++i)
        {

            ifile.read(reinterpret_cast<char*>(&symbol), sizeof(symbol));
            ifile.read(reinterpret_cast<char*>(&count), sizeof(count));

            frequencyMap[symbol] = count;
        }
    }   // END read metadata

    tree.build(frequencyMap);

    {   // decode and write file
        std::string buffer;
        symbol_t    symbol;

        auto traversor = tree.traversor_begin();

        while (ifile.get(symbol))
        {
            std::bitset<SYMBOL_BIT> bits(symbol);
            buffer = std::move(bits.to_string());

            for (symbol_t digit : buffer)
            {
                if (!traversor.is_leaf())
                    traversor += digit;
                else
                {
                    ofile.put(traversor.get_symbol());  // write the character to the output file
                    traversor = tree.traversor_begin(); // reset to root for next character
                }
            }
        }
    }   // END decode and write file

    ifile.close();
    ofile.close();
}

size_t HuffmanCompressor::_compute_file_size(const std::string& filename) const
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate); // open in binary mode and go to the end
    return static_cast<size_t>(file.tellg());
}