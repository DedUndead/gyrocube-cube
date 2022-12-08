#ifndef SHARED_BUFFER_HPP
#define SHARED_BUFFER_HPP


#include <cstring>

class SharedMemoryBuffer{
public:
    SharedMemoryBuffer(const size_t& size);
    ~SharedMemoryBuffer();
    char* insert(char* data, size_t data_size);
private:
    char* buffer;
    uint8_t head;

    size_t _size;
};


#endif
