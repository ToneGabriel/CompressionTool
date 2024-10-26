#include <iostream>

#include "compression/CompressorFacade.h"


int main(int argc, char** args)
{
    // std::string folderPath1 = "H:\\Programare\\C++\\CompressionTool\\input.txt";
    // std::string folderPath2 = "H:\\Programare\\C++\\CompressionTool\\input.bin";

    // CompressorFacade cf;
    // cf.compress(folderPath1, ECompressor::e_HUFFMAN);
    // cf.decompress(folderPath2, ECompressor::e_HUFFMAN);

    // ==============================================================================

    CompressorFacade cf;

    // Paths for input/output files
    std::string inputFilePath = "C:\\Personal\\C++\\CompressionTool\\input.txt";
    std::string compressedFilePath = "C:\\Personal\\C++\\CompressionTool\\compressed.lz77";
    std::string decompressedFilePath = "C:\\Personal\\C++\\CompressionTool\\decompressed.txt";

    // Compress the file
    cf.compress(inputFilePath, compressedFilePath, ECompressor::e_LZ77);
    std::cout << "Compression complete. Compressed data written to " << compressedFilePath << std::endl;

    // Decompress the file
    cf.decompress(compressedFilePath, decompressedFilePath, ECompressor::e_LZ77);
    std::cout << "Decompression complete. Decompressed data written to " << decompressedFilePath << std::endl;

    return 0;
}