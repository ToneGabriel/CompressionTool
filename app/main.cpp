#include <iostream>

#include "../code/encoding/Encoders.h"

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
    SequentialEncoder seqEncoder;

    // Paths for input/output files
    std::string inputFilePath   = "H:\\Programare\\C++\\CompressionTool\\input.txt";
    std::string encodedFilePath = "H:\\Programare\\C++\\CompressionTool\\encoded.bin";
    std::string decodedFilePath = "H:\\Programare\\C++\\CompressionTool\\decoded.txt";

    // Encode the file
    seqEncoder.add_to_sequence(EEncoderType::e_LZ77);
    seqEncoder.encode(inputFilePath);
    seqEncoder.clear_sequence();
    std::cout << "Encoding complete." << std::endl;

    // Decode the file
    seqEncoder.add_to_sequence(EEncoderType::e_LZ77);
    seqEncoder.decode(encodedFilePath);
    seqEncoder.clear_sequence();
    std::cout << "Decoding complete." << std::endl;

    return 0;
}