#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>

#include <vector>
#include <string>
#include <tuple>
#include <deque>

#include "ICompressor.h"
#include "LargeFileOpener.h"


#define USING_DEQUE

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
#ifdef USING_DEQUE
        const int WINDOW_SIZE = 4096;  // Size of the search buffer (sliding window)
        const int LOOKAHEAD_SIZE = 1024; // Size of the lookahead buffer

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

        std::deque<char> searchBuffer;
        std::deque<char> lookaheadBuffer;

        char buffer[LOOKAHEAD_SIZE];

        // Initial read to fill the lookahead buffer
        inputFile.read(buffer, LOOKAHEAD_SIZE);
        lookaheadBuffer.insert(lookaheadBuffer.end(), buffer, buffer + inputFile.gcount());

        while (!lookaheadBuffer.empty())
        {
            LZ77Match match {0, 0, 0};

            // Search for the longest match in the search buffer
            for (int j = 0; j < searchBuffer.size(); ++j)
            {
                int matchLength = 0;
                while (matchLength < lookaheadBuffer.size() &&
                       j + matchLength < searchBuffer.size() &&
                       searchBuffer[j + matchLength] == lookaheadBuffer[matchLength])
                {
                    ++matchLength;
                }

                if (matchLength > match.length)
                {
                    match.offset = searchBuffer.size() - j;
                    match.length = matchLength;
                }
            }

            // Emit match data
            if (match.length < lookaheadBuffer.size())
                match.nextChar = lookaheadBuffer[match.length]; // else nextChar is 0

            // write match
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

            // Refill the lookahead buffer if necessary
            char c;
            while(lookaheadBuffer.size() < LOOKAHEAD_SIZE && !inputFile.eof())
            {
                inputFile.read(reinterpret_cast<char*>(&c), sizeof(c));
                lookaheadBuffer.push_back(c);
            }


            // int shiftSize = match.length + 1;
            // char c;
            // for (int i = 0; i < shiftSize && !inputFile.eof(); ++i)
            // {
            //     if (searchBuffer.size() >= WINDOW_SIZE)
            //         searchBuffer.pop_front(); // Maintain window size
            //     searchBuffer.push_back(lookaheadBuffer.front());

            //     lookaheadBuffer.pop_front();
            //     inputFile.read(reinterpret_cast<char*>(&c), sizeof(c));
            //     lookaheadBuffer.push_back(c);
            // }
        }

        inputFile.close();
        outputFile.close();

#else

        LargeFileOpener inputFile(inputFilePath);
        std::ofstream outputFile(outputFilePath, std::ios::binary);

        const char* fileData    = inputFile.data();
        LONGLONG fileSize       = inputFile.size();
        LONGLONG fileIndex      = 0LL;

        // TODO: compute sizes
        LONGLONG windowSize             = 4096LL;
        LONGLONG lookaheadBufferSize    = 1024LL;

        // compress
        while (fileIndex < fileSize)
        {
            LZ77Match match {0, 0, 0};

            // Find the longest match in the sliding window
            for (LONGLONG i = std::max(0LL, fileIndex - windowSize); i < fileIndex; ++i)    // i from windowStart to fileIndex
            {
                LONGLONG length = 0LL;
                while ( length < lookaheadBufferSize &&
                        fileIndex + length < fileSize &&
                        fileData[i + length] == fileData[fileIndex + length])
                {
                    ++length;
                }

                if (length > match.length)
                {
                    match.length = length;
                    match.offset = fileIndex - i;
                }
            }

            if (fileIndex + match.length < fileSize)
                match.nextChar = fileData[fileIndex + match.length];    // else nextChar is 0

            fileIndex += match.length + 1;

            // write match
            outputFile.write(reinterpret_cast<const char*>(&match.offset), sizeof(match.offset));
            outputFile.write(reinterpret_cast<const char*>(&match.length), sizeof(match.length));
            outputFile.write(reinterpret_cast<const char*>(&match.nextChar), sizeof(match.nextChar));
        }

        inputFile.close();
        outputFile.close();

#endif
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

        std::string decompressedData;
        LZ77Match match;
        
        while (inputFile.read(reinterpret_cast<char*>(&match.offset), sizeof(match.offset)))
        {
            inputFile.read(reinterpret_cast<char*>(&match.length), sizeof(match.length));
            inputFile.read(reinterpret_cast<char*>(&match.nextChar), sizeof(match.nextChar));

            // Handle the match by copying the referenced part of the buffer
            if (match.offset > 0 && match.length > 0)
            {
                int start = decompressedData.size() - match.offset;
                for (int i = 0; i < match.length; ++i)
                {
                    decompressedData += decompressedData[start + i];  // Copy from the history
                }
            }

            // Add the next character if it exists
            if (match.nextChar != 0)
            {
                decompressedData += match.nextChar;
            }
        }

        // Write any remaining decompressed data to the output file
        if (!decompressedData.empty())
        {
            outputFile.write(decompressedData.c_str(), decompressedData.size());
        }

        inputFile.close();
        outputFile.close();
    }

private:

    // std::string decompress(const std::vector<LZ77Match>& compressedData)
    // {
    //     std::string decompressedData;

    //     for (const auto& match : compressedData)
    //     {
    //         if (match.offset > 0 && match.length > 0)
    //         {
    //             int start = decompressedData.size() - match.offset;
    //             for (int i = 0; i < match.length; ++i)
    //             {
    //                 decompressedData += decompressedData[start + i];
    //             }
    //         }

    //         if (match.nextChar != 0)
    //         {
    //             decompressedData += match.nextChar;
    //         }
    //     }

    //     return decompressedData;
    // }
};  // END LZ77Compressor