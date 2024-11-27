#include "HuffmanCompressor.h"

#include <bitset>
#include <queue>

#include <iostream>
#include <fstream>


void HuffmanCompressor::encode(const std::string& inputFilePath, const std::string& outputFilePath)
{
    std::ifstream inputFile(inputFilePath, std::ios::binary);
    std::ofstream outputFile(outputFilePath, std::ios::binary);

    if (!inputFile.is_open() || !outputFile.is_open())
        throw std::runtime_error("Could not open input or output files!");

    size_t                                      padding;
    _HuffmanTree                                tree;
    std::map<symbol_t, size_t>                  frequencyMap;
    std::unordered_map<symbol_t, std::string>   codeMap;

    {   // compute symbol frequencies
        symbol_t symbol;

        while (inputFile.read(reinterpret_cast<char*>(&symbol), sizeof(symbol)))
            ++frequencyMap[symbol];

        inputFile.clear();              // clear error flags
        inputFile.seekg(std::ios::beg); // reset file to beginning
    }

    tree.build(frequencyMap);
    codeMap = tree.generate_huffman_codes();
    padding = _compute_padding(frequencyMap, codeMap);

    {   // write metadata

        // write padding
        outputFile.write(reinterpret_cast<const char*>(&padding), sizeof(padding));

        // write frequency map and its size
        size_t mapSize = frequencyMap.size();
        outputFile.write(reinterpret_cast<const char*>(&mapSize), sizeof(mapSize));

        for (const auto& [symbol, count] : frequencyMap)
        {
            outputFile.write(reinterpret_cast<const char*>(&symbol), sizeof(symbol));
            outputFile.write(reinterpret_cast<const char*>(&count), sizeof(count));
        }
    }   // END write metadata

    {   // encode and write file
        std::string buffer;
        symbol_t    symbol;

        while (inputFile.read(reinterpret_cast<char*>(&symbol), sizeof(symbol)))
        {
            buffer.append(codeMap[symbol]);

            while (buffer.size() >= SYMBOL_BIT)
            {
                // take necessary bits from buffer and transform them into bitset
                std::bitset<SYMBOL_BIT> bits(buffer, 0, SYMBOL_BIT);

                // remaining bits will be used in next iterations
                buffer.erase(0, SYMBOL_BIT);

                // transform bits from bitset into a symbol and write it
                symbol = static_cast<symbol_t>(bits.to_ulong());
                outputFile.write(reinterpret_cast<const char*>(&symbol), sizeof(symbol));
            }
        }

        // pad leftover bits in the buffer with 0s and write them as the last byte
        if (padding != 0)
        {
            // Pad the remaining bits to form a full byte
            buffer.append(padding, SYMBOL_ZERO);
            std::bitset<SYMBOL_BIT> bits(buffer);

            symbol = static_cast<symbol_t>(bits.to_ulong());
            outputFile.write(reinterpret_cast<const char*>(&symbol), sizeof(symbol));
        }
        else
        {
            // do nothing - no padding needed
        }
    }   // END encode and write file
}

void HuffmanCompressor::decode(const std::string& inputFilePath, const std::string& outputFilePath)
{
    std::ifstream inputFile(inputFilePath, std::ios::binary);
    std::ofstream outputFile(outputFilePath, std::ios::binary);

    if (!inputFile.is_open() || !outputFile.is_open())
        throw std::runtime_error("Could not open input or output files!");

    size_t                                      padding;
    _HuffmanTree                                tree;
    std::map<symbol_t, size_t>                  frequencyMap;
    std::unordered_map<symbol_t, std::string>   codeMap;

    {   // read metadata

        // read padding
        inputFile.read(reinterpret_cast<char*>(&padding), sizeof(padding));

        // read frequency map and its size
        size_t      mapSize;
        symbol_t    symbol;
        size_t      count;

        inputFile.read(reinterpret_cast<char*>(&mapSize), sizeof(mapSize));

        for (size_t i = 0; i < mapSize; ++i)
        {

            inputFile.read(reinterpret_cast<char*>(&symbol), sizeof(symbol));
            inputFile.read(reinterpret_cast<char*>(&count), sizeof(count));

            frequencyMap[symbol] = count;
        }
    }   // END read metadata

    tree.build(frequencyMap);

    {   // decode and write file
        std::string buffer;
        symbol_t    symbol;
        size_t      maxLen;

        auto traversor = tree.traversor_begin();

        while (inputFile.read(reinterpret_cast<char*>(&symbol), sizeof(symbol)))
        {
            std::bitset<SYMBOL_BIT> bits(symbol);
            buffer = std::move(bits.to_string());
            maxLen = (inputFile.peek() != EOF) ? SYMBOL_BIT : SYMBOL_BIT - padding;

            for (size_t i = 0; i < maxLen; ++i)
            {
                if (!traversor.is_leaf())
                {
                    traversor += buffer[i];
                }
                else
                {
                    // write the character to the output file
                    symbol = traversor.get_symbol();
                    outputFile.write(reinterpret_cast<const char*>(&symbol), sizeof(symbol));

                    // reset to root for next character
                    traversor = tree.traversor_begin();
                }
            }
        }
    }   // END decode and write file
}

size_t HuffmanCompressor::_compute_padding( const std::map<symbol_t, size_t>& frequencyMap,
                                            const std::unordered_map<symbol_t, std::string>& codeMap)
{
    size_t ret;
    size_t totalCompressedBits  = 0;
    symbol_t leftoverBits       = 0;

    for (auto& [symbol, count] : frequencyMap)
        totalCompressedBits += (count * codeMap.at(symbol).size());

    leftoverBits = totalCompressedBits % SYMBOL_BIT;

    if (leftoverBits == 0)
        ret = 0;
    else
        ret = SYMBOL_BIT - leftoverBits;

    return ret;
}