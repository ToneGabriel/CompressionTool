#include <iostream>

#include "../code/encode/Encoders.h"
// #include "../code/archive/Archive.h"

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
    // Paths for input/output files
    std::string inputFilePath   = std::string(PROJECT_ROOT_PATH) + "/input.txt";
    std::string encodedFilePath = std::string(PROJECT_ROOT_PATH) + "/encoded.txt";
    std::string decodedFilePath = std::string(PROJECT_ROOT_PATH) + "/decoded.txt";

    SequentialEncoder seqEncoder;

    // Encode the file
    seqEncoder.add_to_sequence(EEncoderType::e_LZ77);
    seqEncoder.add_to_sequence(EEncoderType::e_HUFFMAN);
    seqEncoder.encode(inputFilePath, encodedFilePath);
    seqEncoder.clear_sequence();
    std::cout << "Encoding complete." << std::endl;

    // Decode the file
    seqEncoder.add_to_sequence(EEncoderType::e_HUFFMAN);
    seqEncoder.add_to_sequence(EEncoderType::e_LZ77);
    seqEncoder.decode(encodedFilePath, decodedFilePath);
    seqEncoder.clear_sequence();
    std::cout << "Decoding complete." << std::endl;

    return 0;
}