#pragma once

#include <fstream>
#include <cstring>


class Buffered_File_Reader
{
private:
    static constexpr size_t _BUFFER_SIZE = 1024 * 1024; // 1 MB

    char _buffer[_BUFFER_SIZE]; // Fixed-size buffer
    std::ifstream _input_file_stream;
    size_t _buffer_pos = 0; // Current read position
    size_t _buffer_end = 0; // End index of valid data in buffer

public:

    explicit Buffered_File_Reader(const std::string& filename)
    {
        _input_file_stream.open(filename, std::ios::binary);

        if (!_input_file_stream.is_open())
            throw std::runtime_error("Failed to open file: " + filename);
    }

    ~Buffered_File_Reader() = default;

public:

    size_t Get_Max_Read_Count() const
    {
        return _BUFFER_SIZE;
    }

    template<class Container>
    void Read(size_t nchar, Container& output_container)
    {
        _Refill_Buffer_If_Need(nchar);

        if (_Buffer_Current_Size())
        {
            size_t to_read = (std::min)(nchar, _Buffer_Current_Size());
    
            output_container.insert(output_container.end(),
                                    _buffer + _buffer_pos,
                                    _buffer + _buffer_pos + to_read);
    
            _buffer_pos += to_read;
        }
        else
        {
            // do nothing - buffer is empty and eof is reached
        }
    }

    bool EoF() const
    {
        return _Buffer_Current_Size() == 0 && _input_file_stream.eof();
    }

private:

    size_t _Buffer_Current_Size() const
    {
        return _buffer_end - _buffer_pos;
    }

    void _Refill_Buffer_If_Need(size_t next_read_size)  // refill may NOT reach the desired size
    {
        if (next_read_size > _BUFFER_SIZE)
            throw std::out_of_range("Read request exceeds buffer size");

        size_t remaining = _Buffer_Current_Size();

        if (remaining < next_read_size)
        {
            // Move remaining data to the beginning
            std::memmove(_buffer, _buffer + _buffer_pos, remaining);

            // Read new data
            _input_file_stream.read(_buffer + remaining, _BUFFER_SIZE - remaining);

            // Update buffer indices
            _buffer_pos = 0;
            _buffer_end = remaining + _input_file_stream.gcount();;
        }
        else
        {
            // do nothing - no need to refill
        }
    }
}; // END Buffered_File_Reader
