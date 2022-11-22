#include "ring_buffer.hpp"

template <typename T> void ring_buffer<T>::put(T item)
{
    buffer[head] = item;

    if (full) {
        tail = (tail + 1) % max_size;
    }
    head = (head + 1) % max_size;

    full = head == tail;
}

template <typename T> T ring_buffer<T>::get()
{
    if (empty()) return T();  // Return defaul constructor if retrieving

    auto value = buffer[tail];
    
    if (consume) {
        full = false;
        tail = (tail + 1) % max_size;
    }
  
    return value;
}

template <typename T> T* ring_buffer<T>::get_raw_buffer_pointer()
{
    return buffer;
}

template <typename T> void ring_buffer<T>::reset()
{
    head = tail;
    full = false;
}

template <typename T> bool ring_buffer<T>::empty() const
{
    return (!full && (head == tail));
}

template <typename T> bool ring_buffer<T>::full() const
{
    return full;
}

template <typename T> size_t ring_buffer<T>::capacity() const
{
    return max_size;
}

template <typename T> size_t ring_buffer<T>::size() const
{
    if (full) return max_size;

    if (head >= tail) return head - tail;
    else              return max_size + head - tail; 
}
