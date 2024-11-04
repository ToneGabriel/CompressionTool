#include <iostream>

#include "../code/EncoderFacade.h"

// C:\\Personal\\C++\\CompressionTool
// H:\\Programare\\C++\\CompressionTool

int main(int argc, char** args)
{
    EncoderFacade ef;

    // Paths for input/output files
    std::string inputFilePath = "H:\\Programare\\C++\\CompressionTool\\input.txt";
    std::string encodedFilePath = "H:\\Programare\\C++\\CompressionTool\\encoded.bin";
    std::string decodedFilePath = "H:\\Programare\\C++\\CompressionTool\\decoded.txt";

    // Compress the file
    ef.encode(inputFilePath, encodedFilePath, EEncoder::e_MTF);
    std::cout << "Encoding complete. Data written to " << encodedFilePath << std::endl;

    // Decompress the file
    ef.decode(encodedFilePath, decodedFilePath, EEncoder::e_MTF);
    std::cout << "Decoding complete. Data written to " << decodedFilePath << std::endl;

    return 0;
}