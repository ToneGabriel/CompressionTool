#include "Files.h"


namespace fs = std::filesystem;

FILES_BEGIN

std::string create_empty_file_from(const std::string& original, const std::string& newExtension)
{
    fs::path originalPath(original);

    _ASSERT(fs::exists(originalPath), "Original file not found!");

    fs::path newPath = originalPath.replace_extension(newExtension);

    std::ofstream(newPath).close();

    return newPath.string();
}

void read_file_and_process_buffer(const std::string& filename, std::function<void(char*, std::streamsize)> processFunc)
{
    _ASSERT(fs::exists(filename), "File not found!");

    std::ifstream file(filename, std::ios::binary);

    symbol_t buffer[BUFFER_SIZE];

    while (file.read(buffer, BUFFER_SIZE) || file.gcount() > 0)
        processFunc(buffer, file.gcount());

    file.close();
}

FILES_END