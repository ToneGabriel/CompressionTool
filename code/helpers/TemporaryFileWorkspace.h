#pragma once

#include <string>
#include <set>


class TemporaryFileWorkspace
{
private:
    std::set<std::string>   _fileSet;
    std::string             _directoryName;

    size_t                  _maxFileCount;
    size_t                  _currentFileCount;
    size_t                  _incrementalFileIndex;

public:

    TemporaryFileWorkspace(size_t maxFileCount);
    ~TemporaryFileWorkspace();

    TemporaryFileWorkspace(const TemporaryFileWorkspace&) = delete;
    TemporaryFileWorkspace& operator=(const TemporaryFileWorkspace&) = delete;

public:

    std::string create_local_file(std::string extension = "");
    void remove_first_local_file();
    void remove_local_file(const std::string& localfile);
    void move_local_file(const std::string& localfile, const std::string& newPath);
};