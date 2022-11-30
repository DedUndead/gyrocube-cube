#ifndef RING_BUFFER_HPP
#define RING_BUFFER_HPP

/* Not sure how to keep interface/implementation separation practice with templates. */

#include <stddef.h>

#define RING_BUFFER_MAX_SIZE 50

/* 
 * Circular buffer implementation. 
 * The most effective way of handling stream data, e.g., accelerometer sampling
 * Partially adopted from: https://embeddedartistry.com/blog/2017/05/17/creating-a-circular-buffer-in-c-and-c/
 */
template <class T> 
class ring_buffer {
public:
	explicit ring_buffer(size_t size) : max_size(size) { }
	void put(T item);
	T get(bool consume = false);
    T* get_raw_buffer_pointer();
	void reset();
	bool empty() const;
	bool full() const;
	size_t capacity() const;
	size_t size() const;
private:
	T buffer[RING_BUFFER_MAX_SIZE];
	size_t head = 0;
	size_t tail = 0;
	const size_t max_size;
	bool full_ = false;
};

template <typename T>
void ring_buffer<T>::put(T item)
{
    buffer[head] = item;

    if (full_) {
        tail = (tail + 1) % max_size;
    }
    head = (head + 1) % max_size;

    full_ = head == tail;
}

template <typename T>
T ring_buffer<T>::get(bool consume)
{
    if (empty()) return T();  // Return defaul constructor if retrieving

    auto value = buffer[tail];
    
    if (consume) {
        full_ = false;
        tail = (tail + 1) % max_size;
    }
  
    return value;
}

template <typename T>
T* ring_buffer<T>::get_raw_buffer_pointer()
{
    return buffer;
}

template <typename T>
void ring_buffer<T>::reset()
{
    head = tail;
    full_ = false;
}

template <typename T>
bool ring_buffer<T>::empty() const
{
    return (!full_ && (head == tail));
}

template <typename T>
bool ring_buffer<T>::full() const
{
    return full_;
}

template <typename T>
size_t ring_buffer<T>::capacity() const
{
    return max_size;
}

template <typename T>
size_t ring_buffer<T>::size() const
{
    if (full_) return max_size;

    if (head >= tail) return head - tail;
    else              return max_size + head - tail; 
}

#endif
