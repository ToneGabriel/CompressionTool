#include "compression/CompressionDevice.h"


int main(int argc, char** args)
{
    std::string folderPath1 = "H:\\Programare\\C++\\CompressionTool\\input.txt";
    std::string folderPath2 = "H:\\Programare\\C++\\CompressionTool\\input.bin";

    CompressionDevice cd;
    cd.compress(folderPath1, ECompressor::e_HUFFMAN);
    cd.decompress(folderPath2, ECompressor::e_HUFFMAN);

    return 0;
}
