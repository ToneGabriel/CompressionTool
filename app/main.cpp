#include <iostream>

#include "../code/EncoderFacade.h"


// C:\\Personal\\C++\\CompressionTool
// H:\\Programare\\C++\\CompressionTool

int main(int argc, char** args)
{
    // std::string folderPath1 = "H:\\Programare\\C++\\CompressionTool\\input.txt";
    // std::string folderPath2 = "H:\\Programare\\C++\\CompressionTool\\input.bin";

    // EncoderFacade ef;
    // ef.compress(folderPath1, EEncoder::e_HUFFMAN);
    // ef.decompress(folderPath2, EEncoder::e_HUFFMAN);

    // ==============================================================================

    EncoderFacade ef;

    // Paths for input/output files
    std::string inputFilePath = "H:\\Programare\\C++\\CompressionTool\\input.txt";
    std::string compressedFilePath = "H:\\Programare\\C++\\CompressionTool\\compressed.lz77";
    std::string decompressedFilePath = "H:\\Programare\\C++\\CompressionTool\\decompressed.txt";

    // Compress the file
    ef.encode(inputFilePath, compressedFilePath, EEncoder::e_LZ77);
    std::cout << "Compression complete. Compressed data written to " << compressedFilePath << std::endl;

    // Decompress the file
    ef.decode(compressedFilePath, decompressedFilePath, EEncoder::e_LZ77);
    std::cout << "Decompression complete. Decompressed data written to " << decompressedFilePath << std::endl;

    return 0;
}