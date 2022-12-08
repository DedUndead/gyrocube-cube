#include "shared_buffer.hpp"


SharedMemoryBuffer::SharedMemoryBuffer(const size_t& size)
{
    buffer = new char[size];
    head = 0;
    _size = size;
}

SharedMemoryBuffer::~SharedMemoryBuffer()
{
    delete buffer;
}

char* SharedMemoryBuffer::insert(char* data, size_t data_size)
{   
    if (head + data_size >= _size) {
        head = 0;
    }

    memcpy(&buffer[head], data, data_size);
    head += data_size;

    return &buffer[head];
}
