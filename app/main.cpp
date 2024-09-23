#include "huffman/Huffman_Compressor.h"

void write_freq_to_file(const std::string& filename)
{
    std::unordered_map<char, size_t> frequencies;
    frequencies['a'] = 12;
    frequencies['b'] = 34;
    frequencies['c'] = 56;
    frequencies['d'] = 78;
    frequencies['e'] = 90;

    std::ofstream outputFile(filename, std::ios::binary);

    size_t frequencySize = frequencies.size();
    outputFile.write(reinterpret_cast<const char*>(&frequencySize), sizeof(frequencySize));

    // Write each character and its frequency
    for (const auto& pair : frequencies)
    {
        outputFile.put(pair.first); // Write the character
        outputFile.write(reinterpret_cast<const char*>(&pair.second), sizeof(pair.second)); // Write its frequency
    }

    outputFile.close();
}

void read_freq_from_file(const std::string& filename)
{
    std::unordered_map<char, size_t> frequencies;

    std::ifstream inputFile(filename, std::ios::binary);

    size_t frequencySize;
    inputFile.read(reinterpret_cast<char*>(&frequencySize), sizeof(frequencySize));

    // Read each character and its frequency
    for (size_t i = 0; i < frequencySize; ++i)
    {
        char ch;
        size_t frequency;

        inputFile.get(ch); // Read the character
        inputFile.read(reinterpret_cast<char*>(&frequency), sizeof(frequency)); // Read its frequency

        frequencies[ch] = frequency;
    }

    for (auto& val: frequencies)
        std::cout << val.first << ' ' << val.second << '\n';

    inputFile.close();
}


int main(int argc, char** args)
{
    // std::string folderPath = "H:\\Programare\\C++\\CompressionTool\\input";
    // std::string compressedFileName = "H:\\Programare\\C++\\CompressionTool\\compressed.bin";
    // std::string outputFolderPath = "H:\\Programare\\C++\\CompressionTool\\output";

    // huffmanCompressFolder(folderPath, compressedFileName);
    // huffmanDecompressFolder(compressedFileName, outputFolderPath);

    write_freq_to_file("H:\\Programare\\C++\\CompressionTool\\out.txt");
    read_freq_from_file("H:\\Programare\\C++\\CompressionTool\\out.txt");

    return 0;
}
