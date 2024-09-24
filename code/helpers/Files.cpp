#include "Files.h"


FILES_BEGIN

std::string create_empty_file_from(const std::string& original, const std::string& newExtension)
{
    fs::path originalPath(original);

    _ASSERT(fs::exists(originalPath), "Original file not found!");

    fs::path newPath = originalPath.replace_extension(newExtension);

    std::ofstream(newPath).close();

    return newPath.string();
}

std::unordered_map<symbol_t, size_t> get_symbol_frequencies(const std::string& filename)
{
    std::ifstream file(filename, std::ios::binary);

    _ASSERT(file.is_open(), "Error opening file!");

    std::vector<symbol_t>                   buffer(BUFFER_SIZE);
    std::streamsize                         bytesRead;
    std::unordered_map<symbol_t, size_t>    frequencyMap;

    while (file.read(buffer.data(), BUFFER_SIZE) || file.gcount() > 0)
    {
        bytesRead = file.gcount();
        for (std::streamsize i = 0; i < bytesRead; ++i)
        {
            if (!frequencyMap.contains(buffer[i]))
                frequencyMap[buffer[i]] = 0;

            ++frequencyMap[buffer[i]];
        }
    }

    file.close();

    return frequencyMap;
}

FILES_END