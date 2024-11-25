#pragma once

#include <filesystem>

namespace fs = std::filesystem;

class TemporaryFileWorkplace
{
private:
    std::string _workingDirectoryName;
    size_t      _fileIndex = 0;

public:

    TemporaryFileWorkplace()
    {
        fs::path tempDir =  fs::temp_directory_path() /
                            fs::path("temporary_file_workplace_" + std::to_string(std::time(nullptr)));
        fs::create_directory(tempDir);
        _workingDirectoryName = tempDir.string();
    }

    ~TemporaryFileWorkplace()
    {
        // Remove the working directory and its contents
        if (fs::exists(_workingDirectoryName))
            fs::remove_all(_workingDirectoryName);
    }

    TemporaryFileWorkplace(const TemporaryFileWorkplace&) = delete;
    TemporaryFileWorkplace& operator=(const TemporaryFileWorkplace&) = delete;

public:

    std::string create_file(std::string extension = "")
    {
        fs::path tempFilePath = fs::path(_workingDirectoryName) /
                                fs::path("temp_" + std::to_string(_fileIndex++) + extension);
        return tempFilePath.string();
    }

    void remove_file(const std::string& file)
    {

    }
};