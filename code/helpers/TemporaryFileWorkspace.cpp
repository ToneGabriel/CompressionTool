#include "TemporaryFileWorkspace.h"

#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;


TemporaryFileWorkspace::TemporaryFileWorkspace(size_t maxFileCount)
    : _maxFileCount(maxFileCount < 1 ? 1 : maxFileCount), _currentFileCount(0), _incrementalFileIndex(0)
{
    // create path by appending to temp directory the working dir name
    fs::path tempDir =  fs::temp_directory_path() /
                        fs::path("temporary_file_workspace_" + std::to_string(std::time(nullptr)));
    
    // create empty directory
    fs::create_directory(tempDir);

    _directoryName = tempDir.string();
}

TemporaryFileWorkspace::~TemporaryFileWorkspace()
{
    // Remove the working directory and its contents
    if (fs::exists(_directoryName))
        fs::remove_all(_directoryName);
}

std::string TemporaryFileWorkspace::create_local_file(std::string extension)
{
    // create path by appending to working dir the name of the new file
    fs::path tempFilePath = fs::path(_directoryName) /
                            fs::path("temp_file_" + std::to_string(_incrementalFileIndex++) + extension);

    std::string tempFilePathAsString = tempFilePath.string();

    // create empty file
    _fileSet.emplace(tempFilePathAsString);
    ++_currentFileCount;
    std::ofstream(tempFilePathAsString).close();

    // remove oldest file when capacity is exceeded
    if (_currentFileCount > _maxFileCount)
        remove_first_local_file();

    return tempFilePathAsString;
}

void TemporaryFileWorkspace::remove_first_local_file()
{
    if (!_fileSet.empty())
    {
        auto first = _fileSet.begin();

        _fileSet.erase(first);
        --_currentFileCount;

        fs::remove(*first);
    }
}

void TemporaryFileWorkspace::remove_local_file(const std::string& localfile)
{
    fs::path tempFilePath = fs::path(_directoryName) /
                            fs::path(localfile);

    if (fs::exists(tempFilePath))
    {
        auto it = _fileSet.find(tempFilePath.string());

        _fileSet.erase(it);
        --_currentFileCount;

        fs::remove(*it);
    }
}

void TemporaryFileWorkspace::move_local_file(const std::string& localfile, const std::string& newPath)
{
    fs::path tempFilePath = fs::path(_directoryName) /
                            fs::path(localfile);

    if (fs::exists(tempFilePath))
    {
        auto it = _fileSet.find(tempFilePath.string());

        _fileSet.erase(it);
        --_currentFileCount;

        fs::rename(tempFilePath, newPath);
    }
}