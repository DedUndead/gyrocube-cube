#include "ring_buffer.hpp"

template <typename T> void ring_buffer<T>::put(T item)
{
    buffer[head] = item;

    if (full_) {
        tail = (tail + 1) % max_size;
    }
    head = (head + 1) % max_size;

    full_ = head == tail;
}

template <typename T> T ring_buffer<T>::get(bool consume)
{
    if (empty()) return T();  // Return defaul constructor if retrieving

    auto value = buffer[tail];
    
    if (consume) {
        full_ = false;
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
    full_ = false;
}

template <typename T> bool ring_buffer<T>::empty() const
{
    return (!full_ && (head == tail));
}

template <typename T> bool ring_buffer<T>::full() const
{
    return full_;
}

template <typename T> size_t ring_buffer<T>::capacity() const
{
    return max_size;
}

template <typename T> size_t ring_buffer<T>::size() const
{
    if (full_) return max_size;

    if (head >= tail) return head - tail;
    else              return max_size + head - tail; 
}
