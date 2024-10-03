#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>

#include <vector>
#include <string>
#include <tuple>

#include "ICompressor.h"

// TODO


class LZ77Compressor //: public ICompressor
{
public:

    struct Match
    {
        int offset;    // Distance to the previous match in the sliding window
        int length;    // Length of the match
        char nextChar; // The next literal character after the match
    };

    void compressFile(const std::string& inputFilePath, const std::string& outputFilePath, int windowSize, int lookaheadBufferSize)
    {
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

        std::string data((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
        std::vector<Match> compressedData = compress(data, windowSize, lookaheadBufferSize);

        for (const auto& match : compressedData)
        {
            outputFile.write(reinterpret_cast<const char*>(&match.offset), sizeof(match.offset));
            outputFile.write(reinterpret_cast<const char*>(&match.length), sizeof(match.length));
            outputFile.write(&match.nextChar, sizeof(match.nextChar));
        }

        inputFile.close();
        outputFile.close();
    }

    void decompressFile(const std::string& inputFilePath, const std::string& outputFilePath)
    {
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

        std::vector<Match> compressedData;
        Match match;
        while (inputFile.read(reinterpret_cast<char*>(&match.offset), sizeof(match.offset)))
        {
            inputFile.read(reinterpret_cast<char*>(&match.length), sizeof(match.length));
            inputFile.read(&match.nextChar, sizeof(match.nextChar));
            compressedData.push_back(match);
        }

        std::string decompressedData = decompress(compressedData);
        outputFile.write(decompressedData.c_str(), decompressedData.size());

        inputFile.close();
        outputFile.close();
    }

private:

    std::vector<Match> compress(const std::string& data, int windowSize, int lookaheadBufferSize)
    {
        std::vector<Match> compressedData;
        int currentIndex = 0;

        while (currentIndex < data.size())
        {
            int matchOffset = 0;
            int matchLength = 0;

            // Find the longest match in the sliding window
            int startWindow = std::max(0, currentIndex - windowSize);
            for (int i = startWindow; i < currentIndex; ++i)
            {
                int length = 0;
                while (length < lookaheadBufferSize && currentIndex + length < data.size() &&
                       data[i + length] == data[currentIndex + length])
                {
                    ++length;
                }

                if (length > matchLength)
                {
                    matchLength = length;
                    matchOffset = currentIndex - i;
                }
            }

            char nextChar = currentIndex + matchLength < data.size() ? data[currentIndex + matchLength] : '\0';
            compressedData.push_back({matchOffset, matchLength, nextChar});
            currentIndex += matchLength + 1;
        }

        return compressedData;
    }

    std::string decompress(const std::vector<Match>& compressedData)
    {
        std::string decompressedData;

        for (const auto& match : compressedData)
        {
            if (match.offset > 0 && match.length > 0)
            {
                int start = decompressedData.size() - match.offset;
                for (int i = 0; i < match.length; ++i)
                {
                    decompressedData += decompressedData[start + i];
                }
            }

            if (match.nextChar != '\0')
            {
                decompressedData += match.nextChar;
            }
        }

        return decompressedData;
    }
};  // END LZ77Compressor