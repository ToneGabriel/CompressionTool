#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <queue>
#include <bitset>
#include <filesystem>
#include <stdexcept>

#include "Thread_Pool.h"
#include "x_Huffman_Tree.h"

// void generateCodes(_Huffman_Node* root, const std::string& code, std::unordered_map<unsigned char, std::string>& huffmanCode);
// void freeTree(_Huffman_Node* root);
// void writeEncodedToFile(const std::string& encodedStr, std::ofstream& outFile);
// std::string readEncodedFromFile(std::ifstream& inFile);
// void writeTreeToFile(_Huffman_Node* root, std::ofstream& outFile);
// _Huffman_Node* readTreeFromFile(std::ifstream& inFile);
// void huffmanCompressFolder(const std::string& folderPath, const std::string& compressedFileName);
// void huffmanDecompressFolder(const std::string& compressedFileName, const std::string& outputFolderPath);




class Huffman_Compressor
{
private:
    detail::_Huffman_Tree tree;

public:
    // Contructors & Operators
    
    Huffman_Compressor();
    ~Huffman_Compressor();

    Huffman_Compressor(const Huffman_Compressor&)               = delete;
    Huffman_Compressor& operator=(const Huffman_Compressor&)    = delete;

public:
    // Main functions

    void compress(const std::string& dir);
    void decompress(const std::string& dir);

private:
    // Helpers

    void _frequency(const std::string& filename) const;
};  // END Huffman_Compressor
