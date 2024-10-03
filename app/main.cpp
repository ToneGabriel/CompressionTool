#include "compression/CompressionDevice.h"


int main(int argc, char** args)
{
    // std::string folderPath1 = "H:\\Programare\\C++\\CompressionTool\\input.txt";
    // std::string folderPath2 = "H:\\Programare\\C++\\CompressionTool\\input.bin";

    // CompressionDevice cd;
    // cd.compress(folderPath1, ECompressor::e_HUFFMAN);
    // cd.decompress(folderPath2, ECompressor::e_HUFFMAN);

    // ==============================================================================

    // LZ77Compressor lz77;
    // int windowSize = 1024;           // Larger window for more complex data
    // int lookaheadBufferSize = 64;    // Larger buffer for finding matches

    // // Paths for input/output files
    // std::string inputFilePath = "H:\\Programare\\C++\\CompressionTool\\input.txt";
    // std::string compressedFilePath = "H:\\Programare\\C++\\CompressionTool\\compressed.lz77";
    // std::string decompressedFilePath = "H:\\Programare\\C++\\CompressionTool\\decompressed.txt";

    // // Compress the file
    // lz77.compressFile(inputFilePath, compressedFilePath, windowSize, lookaheadBufferSize);
    // std::cout << "Compression complete. Compressed data written to " << compressedFilePath << std::endl;

    // // Decompress the file
    // lz77.decompressFile(compressedFilePath, decompressedFilePath);
    // std::cout << "Decompression complete. Decompressed data written to " << decompressedFilePath << std::endl;

    return 0;
}