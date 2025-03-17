#pragma once

#include <fstream>
#include <cstring>


class BufferedFileReader
{
private:
    static constexpr size_t _s_BUFFER_SIZE = 1024 * 1024; // 1 MB

    char _buffer[_s_BUFFER_SIZE];
    size_t _buffer_pos = 0;
    size_t _buffer_end = 0;

    std::string _input_file_path;
    std::ifstream _input_file_stream;

public:

    explicit BufferedFileReader(const std::string& file)
        : _input_file_path(file)
    {
        Open();
    }

    ~BufferedFileReader()
    {
        Close();
    }

public:

    size_t GetMaxReadCount() const
    {
        return _s_BUFFER_SIZE;
    }

    bool Open()
    {
        if (!_input_file_stream.is_open())
        {
            _input_file_stream.open(_input_file_path, std::ios::binary);

            if (!_input_file_stream.is_open())
                throw std::runtime_error("Failed to open file: " + _input_file_path);
        }
        else
        {
            // do nothing - file is open
        }
    }

    void Close()
    {
        if (_input_file_stream.is_open())
        {
            _input_file_stream.close();

            if (_input_file_stream.is_open())
                throw std::runtime_error("Failed to close file: " + _input_file_path);
        }
        else
        {
            // do nothing - file is closed
        }
    }

    template<class Container>
    bool Read(size_t nchar, Container& output_container)
    {
        _RefillBufferIfNeed(nchar);

        size_t to_read = (std::min)(nchar, _GetBufferCurrentSize());

        if (to_read)
        {
            output_container.insert(output_container.end(),
                                    _buffer + _buffer_pos,
                                    _buffer + _buffer_pos + to_read);
            _buffer_pos += to_read;
        }

        return to_read; // > 0
    }

    bool EoF() const
    {
        return _GetBufferCurrentSize() == 0 && _input_file_stream.eof();
    }

private:

    size_t _GetBufferCurrentSize() const
    {
        return _buffer_end - _buffer_pos;
    }

    void _RefillBufferIfNeed(size_t next_read_size)  // refill may NOT reach the desired size
    {
        if (next_read_size > _s_BUFFER_SIZE)
            throw std::out_of_range("Read request exceeds buffer size");

        size_t remaining = _GetBufferCurrentSize();

        if (remaining < next_read_size)
        {
            // Move remaining data to the beginning
            std::memmove(_buffer, _buffer + _buffer_pos, remaining);

            // Read new data
            _input_file_stream.read(_buffer + remaining, _s_BUFFER_SIZE - remaining);

            // Update buffer indices
            _buffer_pos = 0;
            _buffer_end = remaining + _input_file_stream.gcount();;
        }
        else
        {
            // do nothing - no need to refill
        }
    }
}; // END BufferedFileReader
