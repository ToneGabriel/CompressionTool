#include "TemporaryFileWorkspace.h"

#include <filesystem>

namespace fs = std::filesystem;


TemporaryFileWorkspace::TemporaryFileWorkspace(size_t maxFileCount)
    : _maxFileCount(maxFileCount < 1 ? 1 : maxFileCount), _currentFileCount(0), _incrementalFileIndex(0)
{
    // create path by appending to temp directory the working dir name
    fs::path tempDir =  fs::temp_directory_path() /
                        fs::path("temporary_file_workplace_" + std::to_string(std::time(nullptr)));
    
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

    // this creates an empty file
    fs::resize_file(tempFilePath, 0);
    ++_currentFileCount;

    if (_currentFileCount > _maxFileCount)
        remove_first_local_file();

    return tempFilePath.string();
}

void TemporaryFileWorkspace::remove_first_local_file()
{
    if (!_fileSet.empty())
    {
        auto first = _fileSet.begin();

        fs::remove(*first);
        _fileSet.erase(first);
        --_currentFileCount;
    }
}

void TemporaryFileWorkspace::remove_local_file(const std::string& localfile)
{
    fs::path tempFilePath = fs::path(_directoryName) /
                            fs::path(localfile);

    if (fs::exists(tempFilePath))
    {
        auto it = _fileSet.find(tempFilePath.string());

        fs::remove(*it);
        _fileSet.erase(it);
        --_currentFileCount;
    }
}

void TemporaryFileWorkspace::move_local_file(const std::string& localfile, const std::string& newPath)
{
    // TODO: implement
    fs::path tempFilePath = fs::path(_directoryName) /
                            fs::path(localfile);

    fs::rename(tempFilePath, newPath);
}