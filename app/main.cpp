#include <iostream>

#include "../code/encoding/EncoderFacade.h"

// C:\\Personal\\C++\\CompressionTool
// H:\\Programare\\C++\\CompressionTool

// #define MAIN_ARG_COUNT      5
// #define C_FLAG              "-c"
// #define D_FLAG              "-d"

// #define ARG_COUNT           argc
// #define EXECUTABLE_NAME     args[0]
// #define COMPRESSION_FLAG    args[1]
// #define COMPRESSION_METHOD  args[2]
// #define INPUT_PATH          args[3]
// #define OUTPUT_PATH         args[4]


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

    //std::cout << "Running: " << EXECUTABLE_NAME << '\n';
    // if (ARG_COUNT != MAIN_ARG_COUNT)
    //     return 1;

    // if (COMPRESSION_FLAG == C_FLAG)
    // {
    //     EncoderFacade ef;
    //     ef.encode(INPUT_PATH, OUTPUT_PATH, EEncoder::e_LZ77);
    //     ef.encode(INPUT_PATH, OUTPUT_PATH, EEncoder::e_HUFFMAN);
    // }
    // else if (COMPRESSION_FLAG == D_FLAG)
    // {

    // }
    // else
    //     return 1;

    return 0;
}