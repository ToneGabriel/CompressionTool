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
    seqEncoder.AddToSequence(EEncoderType::e_LZ77);
    seqEncoder.AddToSequence(EEncoderType::e_HUFFMAN);
    seqEncoder.Process(EEncodingDirection::e_FORWARD, inputFilePath, encodedFilePath);
    seqEncoder.ClearSequence();
    std::cout << "Encoding complete." << std::endl;

    // Decode the file
    seqEncoder.AddToSequence(EEncoderType::e_HUFFMAN);
    seqEncoder.AddToSequence(EEncoderType::e_LZ77);
    seqEncoder.Process(EEncodingDirection::e_BACKWARD, encodedFilePath, decodedFilePath);
    seqEncoder.ClearSequence();
    std::cout << "Decoding complete." << std::endl;

    return 0;
}