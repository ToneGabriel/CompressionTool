#include "MTFTransform.h"

#include <iostream>
#include <fstream>
#include <list>


void MTFTransform::encode(const std::string& inputFilePath, const std::string& outputFilePath)
{
    std::ifstream inputFile(inputFilePath, std::ios::binary);
    std::ofstream outputFile(outputFilePath, std::ios::binary);

    if (!inputFile.is_open() || !outputFile.is_open())
        throw std::runtime_error("Could not open input or output files!");

    std::list<char> alphabetList = _create_alphabet_list();

    char symbol;
    while (inputFile.read(&symbol, sizeof(symbol)))
    {
        // Find the symbol and calculate the position in one loop
        int pos = 0;
        auto it = alphabetList.begin();
        auto end = alphabetList.end();
        for (/*empty*/; it != end && *it != symbol; ++it, ++pos) { /* do nothing */ }

        outputFile.write(reinterpret_cast<const char*>(&pos), sizeof(char));    // pos will be 0-255 so its ok to write only 1 byte

        // Move the symbol to the front
        alphabetList.erase(it);
        alphabetList.push_front(symbol);
    }
}

void MTFTransform::decode(const std::string& inputFilePath, const std::string& outputFilePath)
{
    std::ifstream inputFile(inputFilePath, std::ios::binary);
    std::ofstream outputFile(outputFilePath, std::ios::binary);

    if (!inputFile.is_open() || !outputFile.is_open())
        throw std::runtime_error("Could not open input or output files!");

    std::list<char> alphabetList = _create_alphabet_list();

    int pos;
    while (inputFile.read(reinterpret_cast<char*>(&pos), sizeof(char)))
    {
        // Find the symbol at the given position
        auto it = alphabetList.begin();
        std::advance(it, pos);

        char symbol = *it;

        // Write the symbol to the output file
        outputFile.write(&symbol, sizeof(symbol));

        // Move the symbol to the front
        alphabetList.erase(it);
        alphabetList.push_front(symbol);
    }
}

std::list<char> MTFTransform::_create_alphabet_list() const
{
    std::list<char> ret;

    // Initialize positions list to hold symbols 0-255
    for (int i = 0; i < 256; ++i)
        ret.push_back(static_cast<char>(i));

    return ret;
}