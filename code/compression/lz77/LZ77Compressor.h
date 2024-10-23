#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>

#include <vector>
#include <string>
#include <tuple>
#include <deque>

#include "ICompressor.h"


class LZ77Compressor //: public ICompressor
{
private:

    struct LZ77Match
    {
        int offset;
        int length;
        char nextChar;
    };

public:

    void compressFile(const std::string& inputFilePath, const std::string& outputFilePath)
    {
        const int WINDOW_SIZE = 4096;  // Size of the search buffer (sliding window)
        const int LOOKAHEAD_SIZE = 1024; // Size of the lookahead buffer

        std::deque<char> searchBuffer;
        std::deque<char> lookaheadBuffer;
        char readBuffer[LOOKAHEAD_SIZE];

        std::ifstream inputFile(inputFilePath, std::ios::binary);
        if (!inputFile.is_open())
        {
            std::cerr << "Error: Could not open file " << inputFilePath << std::endl;
            return;
        }

        std::ofstream outputFile(outputFilePath, std::ios::binary);
        if (!outputFile.is_open())
        {
            std::cerr << "Error: Could not open file " << outputFilePath << std::endl;
            return;
        }

        // Initial read to fill the lookahead buffer
        inputFile.read(readBuffer, LOOKAHEAD_SIZE);
        lookaheadBuffer.insert(lookaheadBuffer.end(), readBuffer, readBuffer + inputFile.gcount());

        while (!lookaheadBuffer.empty())
        {
            LZ77Match match {0, 0, 0};

            // Search for the longest match in the search buffer
            for (int i = 0; i < searchBuffer.size(); ++i)
            {
                int matchLength = 0;
                while (matchLength < lookaheadBuffer.size() &&
                       i + matchLength < searchBuffer.size() &&
                       searchBuffer[i + matchLength] == lookaheadBuffer[matchLength])
                {
                    ++matchLength;
                }

                if (matchLength > match.length)
                {
                    match.offset = searchBuffer.size() - i;
                    match.length = matchLength;
                }
            }

            if (match.length < lookaheadBuffer.size())
                match.nextChar = lookaheadBuffer[match.length]; // else nextChar is 0

            // Write match
            outputFile.write(reinterpret_cast<const char*>(&match.offset), sizeof(match.offset));
            outputFile.write(reinterpret_cast<const char*>(&match.length), sizeof(match.length));
            outputFile.write(reinterpret_cast<const char*>(&match.nextChar), sizeof(match.nextChar));

            // Slide the window
            int shiftSize = match.length + 1;
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

        inputFile.close();
        outputFile.close();
    }

    void decompressFile(const std::string& inputFilePath, const std::string& outputFilePath)
    {
        const int WINDOW_SIZE = 4096;

        std::ifstream inputFile(inputFilePath, std::ios::binary);
        if (!inputFile.is_open())
        {
            std::cerr << "Could not open the input file." << std::endl;
            return;
        }

        std::ofstream outputFile(outputFilePath, std::ios::binary);
        if (!outputFile.is_open())
        {
            std::cerr << "Could not open the output file." << std::endl;
            return;
        }

        LZ77Match match;
        std::deque<char> decompressedBuffer;

        while (inputFile.read(reinterpret_cast<char*>(&match.offset), sizeof(match.offset)))
        {
            inputFile.read(reinterpret_cast<char*>(&match.length), sizeof(match.length));
            inputFile.read(reinterpret_cast<char*>(&match.nextChar), sizeof(match.nextChar));

            // Write the match from the history (search buffer) directly to the output file
            if (match.offset > 0 && match.length > 0)
            {
                char c;
                int start = decompressedBuffer.size() - match.offset;
                for (int i = 0; i < match.length; ++i)
                {
                    c = decompressedBuffer[start + i];
                    outputFile.write(&c, sizeof(c));  // Write directly to the file
                    decompressedBuffer.push_back(c);  // Add to the search buffer
                }
            }

            // Write the next literal character directly to the output file
            if (match.nextChar != 0)
            {
                outputFile.write(&match.nextChar, sizeof(match.nextChar));
                decompressedBuffer.push_back(match.nextChar);  // Add to the search buffer
            }

            // Maintain a maximum buffer size to simulate the sliding window
            while (decompressedBuffer.size() > WINDOW_SIZE)
            {
                decompressedBuffer.pop_front();  // Remove old entries from the buffer
            }
        }

        inputFile.close();
        outputFile.close();
    }
};  // END LZ77Compressor