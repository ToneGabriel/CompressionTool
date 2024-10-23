#pragma once

#include <windows.h>
#include <string>
#include <functional>

#include "_Core.h"


class LargeFileOpener
{
private:

    HANDLE  _fileHandle = nullptr;
    HANDLE  _mapHandle  = nullptr;
    char*   _data       = nullptr;

public:

    LargeFileOpener() = delete;

    LargeFileOpener(const std::string& filename)
    {
        open(filename);
    }

    ~LargeFileOpener()
    {
        close();
    }

public:

    void open(const std::string& filename)
    {
        if (is_open())
            return;

        // create file handler
        _fileHandle = CreateFile(   filename.c_str(),
                                    GENERIC_READ,
                                    0,
                                    nullptr,
                                    OPEN_EXISTING,
                                    FILE_ATTRIBUTE_NORMAL,
                                    nullptr);

        if (_fileHandle == INVALID_HANDLE_VALUE)
        {
            throw std::runtime_error("File could not be opened!");
        }

        // create map handler
        _mapHandle = CreateFileMapping( _fileHandle,
                                        nullptr,
                                        PAGE_READONLY,
                                        0,
                                        0,
                                        nullptr);

        if (!_mapHandle)
        {
            CloseHandle(_fileHandle);
            throw std::runtime_error("File could not be mapped!");
        }

        // initialize file data view
        _data = static_cast<char*>(MapViewOfFile(   _mapHandle,
                                                    FILE_MAP_READ,
                                                    0,
                                                    0,
                                                    0));

        if (!_data)
        {
            CloseHandle(_mapHandle);
            CloseHandle(_fileHandle);
            throw std::runtime_error("File data could not be initialized!");
        }
    }

    void close()
    {
        if (!is_open())
            return;

        UnmapViewOfFile(_data);
        CloseHandle(_mapHandle);
        CloseHandle(_fileHandle);

        _data       = nullptr;
        _mapHandle  = nullptr;
        _fileHandle = nullptr;
    }

    bool is_open() const
    {
        return _data != nullptr;
    }

    LONGLONG size() const
    {
        LARGE_INTEGER fileSize;
        GetFileSizeEx(_fileHandle, &fileSize);
        return fileSize.QuadPart;
    }

    const char* data() const
    {
        return _data;
    }
};  // END LargeFileOpener