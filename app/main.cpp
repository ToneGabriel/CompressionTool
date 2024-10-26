#include "compression/CompressorFactory.h"


int main(int argc, char** args)
{
    // std::string folderPath1 = "H:\\Programare\\C++\\CompressionTool\\input.txt";
    // std::string folderPath2 = "H:\\Programare\\C++\\CompressionTool\\input.bin";

    // CompressorFactory cf;
    // cf.compress(folderPath1, ECompressor::e_HUFFMAN);
    // cf.decompress(folderPath2, ECompressor::e_HUFFMAN);

    // ==============================================================================

    LZ77Compressor lz77;

    // Paths for input/output files
    std::string inputFilePath = "C:\\Personal\\C++\\CompressionTool\\input.txt";
    std::string compressedFilePath = "C:\\Personal\\C++\\CompressionTool\\compressed.lz77";
    std::string decompressedFilePath = "C:\\Personal\\C++\\CompressionTool\\decompressed.txt";

    // Compress the file
    lz77.compress(inputFilePath, compressedFilePath);
    std::cout << "Compression complete. Compressed data written to " << compressedFilePath << std::endl;

    // Decompress the file
    lz77.decompress(compressedFilePath, decompressedFilePath);
    std::cout << "Decompression complete. Decompressed data written to " << decompressedFilePath << std::endl;

    return 0;
}