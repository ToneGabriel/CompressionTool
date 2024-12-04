#include "LZ77Compressor.h"

#include <iostream>
#include <fstream>

#include <string>
#include <deque>


void LZ77Compressor::encode(const std::string& inputFilePath, const std::string& outputFilePath)
{
    std::ifstream inputFile(inputFilePath, std::ios::binary);
    std::ofstream outputFile(outputFilePath, std::ios::binary);

    if (!inputFile.is_open() || !outputFile.is_open())
        throw std::runtime_error("Could not open input or output files!");

    std::deque<char> searchBuffer;
    std::deque<char> lookaheadBuffer;
    char readBuffer[LOOKAHEAD_SIZE];

    // Initial read to fill the lookahead buffer
    inputFile.read(readBuffer, LOOKAHEAD_SIZE);
    lookaheadBuffer.insert(lookaheadBuffer.end(), readBuffer, readBuffer + inputFile.gcount());

    while (!lookaheadBuffer.empty())
    {
        _LZ77Match match {0, 0, 0};

        {   // Search for the longest match in the search buffer
            int searchBufferSize    = searchBuffer.size();
            int lookaheadBufferSize = lookaheadBuffer.size();
            int matchLength;

            for (int i = 0; i < searchBufferSize; ++i)
            {
                matchLength = 0;
                while ( matchLength < lookaheadBufferSize &&
                        i + matchLength < searchBufferSize &&
                        searchBuffer[i + matchLength] == lookaheadBuffer[matchLength])
                {
                    ++matchLength;
                }

                if (matchLength > match._Length)
                {
                    match._Offset = searchBufferSize - i;
                    match._Length = matchLength;
                }
            }

            if (match._Length < lookaheadBufferSize)
                match._NextChar = lookaheadBuffer[match._Length];
            else
            {
                // do nothing - _NextChar remains 0
            }   
        }   // END - Search for the longest match in the search buffer

        // Write match
        outputFile.write(reinterpret_cast<const char*>(&match._Offset), sizeof(match._Offset));
        outputFile.write(reinterpret_cast<const char*>(&match._Length), sizeof(match._Length));
        outputFile.write(reinterpret_cast<const char*>(&match._NextChar), sizeof(match._NextChar));

        // Slide the window
        int shiftSize = match._Length + 1;
        for (int i = 0; i < shiftSize && !lookaheadBuffer.empty(); ++i)
        {
            if (searchBuffer.size() >= WINDOW_SIZE)
                searchBuffer.pop_front(); // Maintain window size

            searchBuffer.push_back(lookaheadBuffer.front());
            lookaheadBuffer.pop_front();
        }

        // Refill the lookahead buffer
        if (lookaheadBuffer.size() < LOOKAHEAD_SIZE)
        {
            inputFile.read(readBuffer, LOOKAHEAD_SIZE - lookaheadBuffer.size());
            lookaheadBuffer.insert(lookaheadBuffer.end(), readBuffer, readBuffer + inputFile.gcount());
        }
    }
}

void LZ77Compressor::decode(const std::string& inputFilePath, const std::string& outputFilePath)
{
    std::ifstream inputFile(inputFilePath, std::ios::binary);
    std::ofstream outputFile(outputFilePath, std::ios::binary);

    if (!inputFile.is_open() || !outputFile.is_open())
        throw std::runtime_error("Could not open input or output files!");

    _LZ77Match match;
    std::deque<char> decompressedBuffer;

    while ( inputFile.read(reinterpret_cast<char*>(&match._Offset), sizeof(match._Offset)) &&
            inputFile.read(reinterpret_cast<char*>(&match._Length), sizeof(match._Length)) &&
            inputFile.read(reinterpret_cast<char*>(&match._NextChar), sizeof(match._NextChar)))
    {
        // Write the match from the history (decompressed buffer) directly to the output file
        if (match._Offset > 0 && match._Length > 0)
        {
            char c;
            int start = decompressedBuffer.size() - match._Offset;
            for (int i = 0; i < match._Length; ++i)
            {
                c = decompressedBuffer[start + i];
                outputFile.write(&c, sizeof(c));  // Write directly to the file
                decompressedBuffer.push_back(c);  // Add to the search buffer
            }
        }

        // Write the next literal character directly to the output file
        if (match._NextChar != 0)
        {
            outputFile.write(&match._NextChar, sizeof(match._NextChar));
            decompressedBuffer.push_back(match._NextChar);  // Add to the search buffer
        }

        // Maintain a maximum buffer size to simulate the sliding window
        while (decompressedBuffer.size() > WINDOW_SIZE)
            decompressedBuffer.pop_front();  // Remove old entries from the buffer
    }
}